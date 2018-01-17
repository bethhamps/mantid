from __future__ import absolute_import, print_function

from mantid.api import AlgorithmObserver


class ProgressObserver(AlgorithmObserver):
    """
    Observes a single algorithm for its progress and finish notifications
    and updates presenter accordingly.
    """
    def __init__(self, model, alg):
        super(ProgressObserver, self).__init__()
        self.model = model
        self.algorithm = alg
        self.message = None
        self.progress = 0.0

    def name(self):
        return self.algorithm.name()

    def properties(self):
        """
        Get algorithm properties.
        :return: A list of objects with fields: name, value, isDefault.
        """
        return self.algorithm.getProperties()

    def finishHandle(self):
        self.model.remove_observer(self)

    def progressHandle(self, p, message):
        self.progress = p
        if len(message) > 0:
            self.message = message
        else:
            self.message = None
        self.model.update_progress(self)


class AlgorithmProgressModel(AlgorithmObserver):
    """
    Observes AlgorithmManager for new algorithms and starts
    ProgressObservers.
    """
    def __init__(self):
        super(AlgorithmProgressModel, self).__init__()
        self.presenters = []
        self.progress_observers = []
        self.observeStarting()

    def add_presenter(self, presenter):
        self.presenters.append(presenter)

    def remove_presenter(self, presenter):
        index = self.presenters.index(presenter)
        if index >= 0:
            del self.presenters[index]

    def startingHandle(self, alg):
        progress_observer = ProgressObserver(self, alg)
        progress_observer.observeProgress(alg)
        progress_observer.observeFinish(alg)
        self.progress_observers.append(progress_observer)
        for presenter in self.presenters:
            presenter.close_progress_bar()

    def update_progress(self, progress_observer):
        """
        Update the progress bar in the view.
        :param progress_observer: A observer reporting a progress.
        """
        # self.presenter.update_progress_bar(progress_observer.progress, progress_observer.message)
        pass

    def remove_observer(self, progress_observer):
        """
        Remove the progress observer when it's finished
        :param progress_observer: A ProgressObserver that needs removing
        """
        index = self.progress_observers.index(progress_observer)
        if index >= 0:
            del self.progress_observers[index]
            for presenter in self.presenters:
                presenter.close_progress_bar()

    def get_running_algorithm_data(self):
        """
        Create data describing running algorithms.
        :return: A list of tuples of two elements: first one is the algorithm's name
            and the second is a list of algorithm property descriptors - 3-element lists of the form:
                [prop_name, prop_value_as_str, 'Default' or '']
        """
        algorithm_data = []
        for observer in self.progress_observers:
            properties = []
            for prop in observer.properties():
                properties.append([prop.name, str(prop.value), 'Default' if prop.isDefault else ''])
            algorithm_data.append((observer.name(), properties))
        return algorithm_data
