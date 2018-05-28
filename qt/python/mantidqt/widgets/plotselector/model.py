#  This file is part of the mantid workbench.
#
#  Copyright (C) 2018 mantidproject
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
from __future__ import absolute_import, print_function

class PlotSelectorModel(object):
    """
    This is the model for the plot selector widget. Essentially this
    is just a thin wrapper to the true model - CurrentFigure.
    """

    def __init__(self, presenter, current_figure_class):
        """
        Initialise a new instance of PlotSelectorModel
        :param presenter: A presenter controlling this model.
        """
        self.CurrentFigure = current_figure_class
        self.presenter = presenter
        self.plot_list = []

        # Register with CurrentFigure that we want to know of any
        # changes to the list of plots
        self.CurrentFigure.add_observer(self)

    def update_plot_list(self):
        self.plot_list = []
        figures = self.CurrentFigure.get_all_fig_managers()
        for figure in figures:
            self.plot_list.append(figure.get_window_title())

    def notify(self):
        self.presenter.update_plot_list()

    def make_plot_active(self, plot_name):
        self.CurrentFigure.bring_to_front_by_name(plot_name)

    def close_plot(self, plot_name):
        figure_number_to_close = self.CurrentFigure.get_figure_number_from_name(plot_name)
        self.CurrentFigure.destroy(figure_number_to_close)
