/*=========================================================================

   Program: ParaView
   Module:    pqHelpWindow.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "MantidQtWidgets/Common/pqHelpWindow.h"
#include "MantidQtWidgets/Common/MantidDesktopServices.h"
#include "ui_pqHelpWindow.h"

#include <QFileInfo>
#include <QHelpContentWidget>
#include <QHelpEngine>
#include <QHelpIndexWidget>
#include <QHelpSearchQueryWidget>
#include <QHelpSearchResultWidget>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QPointer>
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextStream>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QUrl>

// ****************************************************************************
//            CLASS pqHelpWindowNetworkReply
// ****************************************************************************
/// Internal class used to add support to QWeb(Engine)View to load files from
/// QHelpEngine.
class pqHelpWindowNetworkReply : public QNetworkReply {
  using Superclass = QNetworkReply;

public:
  pqHelpWindowNetworkReply(const QUrl &url, QHelpEngineCore *helpEngine,
                           QObject *parent = nullptr);

  void abort() override {}

  qint64 bytesAvailable() const override {
    return (this->RawData.size() - this->Offset) +
           this->Superclass::bytesAvailable();
  }
  bool isSequential() const override { return true; }

protected:
  qint64 readData(char *data, qint64 maxSize) override;

  QByteArray RawData;
  qint64 Offset;

private:
  Q_DISABLE_COPY(pqHelpWindowNetworkReply)
};

//-----------------------------------------------------------------------------
pqHelpWindowNetworkReply::pqHelpWindowNetworkReply(const QUrl &my_url,
                                                   QHelpEngineCore *engine,
                                                   QObject *parent)
    : Superclass(parent), Offset(0) {
  Q_ASSERT(engine);

  this->RawData = engine->fileData(my_url);

  QString content_type = "text/plain";
  QString extension = QFileInfo(my_url.path()).suffix().toLower();
  QMap<QString, QString> extension_type_map;
  extension_type_map["jpg"] = "image/jpeg";
  extension_type_map["jpeg"] = "image/jpeg";
  extension_type_map["png"] = "image/png";
  extension_type_map["gif"] = "image/gif";
  extension_type_map["tiff"] = "image/tiff";
  extension_type_map["htm"] = "text/html";
  extension_type_map["html"] = "text/html";
  extension_type_map["css"] = "text/css";
  extension_type_map["xml"] = "text/xml";

  if (extension_type_map.contains(extension)) {
    content_type = extension_type_map[extension];
  }

  this->setHeader(QNetworkRequest::ContentLengthHeader,
                  QVariant(this->RawData.size()));
  this->setHeader(QNetworkRequest::ContentTypeHeader, content_type);
  this->open(QIODevice::ReadOnly | QIODevice::Unbuffered);
  this->setUrl(my_url);
  QTimer::singleShot(0, this, SIGNAL(readyRead()));
  QTimer::singleShot(0, this, SLOT(finished()));
}

//-----------------------------------------------------------------------------
qint64 pqHelpWindowNetworkReply::readData(char *data, qint64 maxSize) {
  if (this->Offset <= this->RawData.size()) {
    qint64 end =
        qMin(this->Offset + maxSize, static_cast<qint64>(this->RawData.size()));
    qint64 delta = end - this->Offset;
    memcpy(data, this->RawData.constData() + this->Offset, delta);
    this->Offset += delta;
    return delta;
  }
  return -1;
}

#if defined(USE_QTWEBKIT)
#include <QWebHistory>
#include <QWebView>

// ****************************************************************************
//    CLASS pqHelpWindow::pqNetworkAccessManager
// ****************************************************************************
//-----------------------------------------------------------------------------
class pqHelpWindow::pqNetworkAccessManager : public QNetworkAccessManager {
  using Superclass = QNetworkAccessManager;
  QPointer<QHelpEngineCore> Engine;

public:
  pqNetworkAccessManager(QHelpEngineCore *helpEngine,
                         QNetworkAccessManager *manager, QObject *parentObject)
      : Superclass(parentObject), Engine(helpEngine) {
    Q_ASSERT(manager != nullptr && helpEngine != nullptr);

    this->setCache(manager->cache());
    this->setCookieJar(manager->cookieJar());
    this->setProxy(manager->proxy());
    this->setProxyFactory(manager->proxyFactory());
  }

protected:
  QNetworkReply *createRequest(Operation operation,
                               const QNetworkRequest &request,
                               QIODevice *device) override {
    if (request.url().scheme() == "qthelp" && operation == GetOperation) {
      return new pqHelpWindowNetworkReply(request.url(), this->Engine, this);
    } else {
      return this->Superclass::createRequest(operation, request, device);
    }
  }

private:
  Q_DISABLE_COPY(pqNetworkAccessManager)
};

#else

#include <QWebEngineHistory>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineView>

/// Adds support for qthelp scheme links that load content from them QHelpEngine
class QtHelpUrlHandler : public QWebEngineUrlSchemeHandler {
public:
  QtHelpUrlHandler(QHelpEngineCore *helpEngine, QObject *parent = nullptr)
      : QWebEngineUrlSchemeHandler(parent), Engine(helpEngine) {}

protected:
  void requestStarted(QWebEngineUrlRequestJob *request) override {
    request->reply("text/html",
                   new pqHelpWindowNetworkReply(request->requestUrl(),
                                                this->Engine, request));
  }

private:
  QHelpEngineCore *Engine;
};

#endif

// ****************************************************************************
//            CLASS pqHelpWindow
// ****************************************************************************

//-----------------------------------------------------------------------------
pqHelpWindow::pqHelpWindow(QHelpEngine *engine, QWidget *parentObject,
                           Qt::WindowFlags parentFlags)
    : Superclass(parentObject, parentFlags), m_helpEngine(engine) {
  Q_ASSERT(engine != nullptr);
  // Take ownership of the engine
  m_helpEngine->setParent(this);

  Ui::pqHelpWindow ui;
  ui.setupUi(this);

  // all warnings from the help engine get logged
  QObject::connect(this->m_helpEngine, SIGNAL(warning(const QString &)), this,
                   SIGNAL(helpWarnings(const QString &)));

  // add a navigation toolbar
  QToolBar *navigation = new QToolBar("Navigation");
  QPushButton *home = new QPushButton("Home");
  QPushButton *print = new QPushButton("Print...");
  print->setToolTip("Print the current page");

  m_forward = new QToolButton();
  m_forward->setArrowType(Qt::RightArrow);
  m_forward->setToolTip("next");
  m_forward->setEnabled(false);
  m_forward->setAutoRaise(true);

  m_backward = new QToolButton();
  m_backward->setArrowType(Qt::LeftArrow);
  m_backward->setToolTip("previous");
  m_backward->setEnabled(false);
  m_backward->setAutoRaise(true);

  navigation->addWidget(home);
  navigation->addWidget(print);
  navigation->addWidget(m_backward);
  navigation->addWidget(m_forward);
  navigation->setAllowedAreas(Qt::TopToolBarArea | Qt::RightToolBarArea);
  this->addToolBar(navigation);

  this->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

  // create index and search dock tabs
  this->tabifyDockWidget(ui.indexDock, ui.searchDock);
  ui.indexDock->setWidget(this->m_helpEngine->indexWidget());

  // setup the search tab
  QWidget *searchPane = new QWidget(this);
  QVBoxLayout *vbox = new QVBoxLayout();
  searchPane->setLayout(vbox);
  vbox->addWidget(this->m_helpEngine->searchEngine()->queryWidget());
  vbox->addWidget(this->m_helpEngine->searchEngine()->resultWidget());
  connect(this->m_helpEngine->searchEngine()->resultWidget(),
          SIGNAL(requestShowLink(QUrl)), this, SLOT(showPage(QUrl)));

  // set the search connection
  ui.searchDock->setWidget(searchPane);
  connect(this->m_helpEngine->searchEngine()->queryWidget(), SIGNAL(search()),
          this, SLOT(search()));

  // connect the index page to the content pane
  connect(m_helpEngine->contentWidget(), SIGNAL(linkActivated(QUrl)), this,
          SLOT(showPage(QUrl)));
  connect(this->m_helpEngine->indexWidget(),
          SIGNAL(linkActivated(QUrl, QString)), this, SLOT(showPage(QUrl)));

// setup the content pane
#if defined(USE_QTWEBKIT)
  m_browser = new QWebView(this);
  QNetworkAccessManager *oldManager = m_browser->page()->networkAccessManager();
  pqNetworkAccessManager *newManager =
      new pqNetworkAccessManager(m_helpEngine, oldManager, this);
  m_browser->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  m_browser->page()->setNetworkAccessManager(newManager);
  m_browser->page()->setForwardUnsupportedContent(false);
  connect(m_browser, SIGNAL(linkClicked(QUrl)), this, SLOT(showPage(QUrl)));
  // set up the status bar
  connect(m_browser->page(), SIGNAL(linkHovered(QString, QString, QString)),
          this, SLOT(linkHovered(QString, QString, QString)));
#else
  QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(
      "qthelp", new QtHelpUrlHandler(engine));
  m_browser = new QWebEngineView(this);
  m_browser->setPage(new DelegatingWebPage(m_browser));
  connect(m_browser->page(), SIGNAL(linkClicked(QUrl)), this,
          SLOT(showLinkedPage(QUrl)));
  // set up the status bar
  connect(m_browser->page(), SIGNAL(linkHovered(QString)), this,
          SLOT(linkHovered(QString)));
#endif
  this->setCentralWidget(this->m_browser);

  // connect the navigation buttons
  connect(home, SIGNAL(clicked()), this, SLOT(showHomePage()));
  connect(print, SIGNAL(clicked()), this, SLOT(printPage()));
  connect(m_forward, SIGNAL(clicked()), m_browser, SLOT(forward()));
  connect(m_backward, SIGNAL(clicked()), m_browser, SLOT(back()));
  connect(m_forward, SIGNAL(clicked()), this, SLOT(updateNavButtons()));
  connect(m_backward, SIGNAL(clicked()), this, SLOT(updateNavButtons()));

  // setup the search engine to do its job
  m_helpEngine->searchEngine()->reindexDocumentation();
}

//-----------------------------------------------------------------------------

/**
 * Set the contents of the browser to show an error message.
 * @param url The url that could not be found.
 */
void pqHelpWindow::errorMissingPage(const QUrl &url) {
  QString htmlDoc =
      QString(QLatin1String(
                  "<html><head><title>Invalid Url - %1</title></head><body>"))
          .arg(url.toString());

  htmlDoc +=
      QString(QLatin1String("<center><h1>Missing page - %1</h1></center>"))
          .arg(url.toString());

  htmlDoc += QLatin1String("</body></html>");

  m_browser->setHtml(htmlDoc);
}

//-----------------------------------------------------------------------------
void pqHelpWindow::showPage(const QString &url,
                            bool linkClicked /* = false */) {
  this->showPage(QUrl::fromUserInput(url), linkClicked);
}

//-----------------------------------------------------------------------------
void pqHelpWindow::showPage(const QUrl &url, bool linkClicked /* = false */) {
  if (url.scheme() == "qthelp") {
    if (this->m_helpEngine->findFile(url).isValid()) {
      if (!linkClicked)
        this->m_browser->setUrl(url);
    } else {
      errorMissingPage(url);
    }
    if (m_browser->history()->count() > 0)
      m_backward->setEnabled(true);
    m_forward->setEnabled(false);
  } else {
    using MantidQt::API::MantidDesktopServices;
    MantidDesktopServices::openUrl(url);
  }
}

//-----------------------------------------------------------------------------
void pqHelpWindow::showLinkedPage(const QUrl &url) {
  this->showPage(url, true);
}

//-----------------------------------------------------------------------------
void pqHelpWindow::printPage() {
  QPrinter printer;
  QPrintDialog dialog(&printer, this);
  dialog.setWindowTitle(tr("Print Document"));
  if (dialog.exec() != QDialog::Accepted)
    return;
#if defined(USE_QTWEBKIT)
  m_browser->print(&printer);
#else
  m_browser->page()->print(&printer, [](bool) {});
#endif
}

//-----------------------------------------------------------------------------
void pqHelpWindow::updateNavButtons() {
  m_forward->setEnabled(m_browser->history()->canGoForward());
  m_backward->setEnabled(m_browser->history()->canGoBack());
}

//-----------------------------------------------------------------------------
void pqHelpWindow::search() {
  QList<QHelpSearchQuery> query =
      this->m_helpEngine->searchEngine()->queryWidget()->query();
  this->m_helpEngine->searchEngine()->search(query);
}

//-----------------------------------------------------------------------------
void pqHelpWindow::linkHovered(const QString &link, const QString &title,
                               const QString &textContent) {
  (void)title;
  (void)textContent;
  this->statusBar()->showMessage(link);
}

void pqHelpWindow::showHomePage() {
  showPage(QString("qthelp://org.mantidproject/doc/index.html"));
}

//-----------------------------------------------------------------------------
void pqHelpWindow::showHomePage(const QString &namespace_name) {
  QList<QUrl> html_pages =
      this->m_helpEngine->files(namespace_name, QStringList(), "html");
  // now try to locate a file named index.html in this collection.
  foreach (QUrl url, html_pages) {
    if (url.path().endsWith("index.html")) {
      this->showPage(url.toString());
      return;
    }
  }
  errorMissingPage(QUrl("Could not locate index.html"));
}
