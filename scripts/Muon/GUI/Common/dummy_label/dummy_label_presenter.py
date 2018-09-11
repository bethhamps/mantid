from __future__ import (absolute_import, division, print_function)


class DummyLabelPresenter(object):
    """
    """
    def __init__(self,view,model):
        self._view=view
        self._model=model

    @property
    def widget(self):
        return self._view

    @property
    def model(self):
        return self._model

    def updateLabel(self,message):
        self._view.updateLabel(message)

    # interact with context
    def updateContext(self):
      """
      A simple method for updating the context.
      Gets the current values from the view and the model
      will update the context accordingly (inc. packing the data)
      """
      subcontext = self._view.getSubContext()
      self._model.updateContext(subcontext)
