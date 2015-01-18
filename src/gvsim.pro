

TEMPLATE = subdirs

gvsim_simengine.subdir = gvsim-simengine
gvsim_simengine.target = gvsim-simengine

gvsim_gui.subdir = gvsim-gui
gvsim_gui.target = gvsim
gvsim_gui.depends = gvsim-simengine

SUBDIRS = gvsim_simengine gvsim_gui
