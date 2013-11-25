#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T10:42:31
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Spike_Sotring_Offline_GUI
TEMPLATE = app


SOURCES += main.cpp\
    CALCUL/enginesorting.cpp \
    CALCUL/filtering/filter.cpp \
    CALCUL/readdata.cpp \
    CALCUL/librairieAlglib/ap.cpp \
    CALCUL/librairieAlglib/alglibinternal.cpp \
    CALCUL/librairieAlglib/statistics.cpp \
    CALCUL/librairieAlglib/specialfunctions.cpp \
    CALCUL/librairieAlglib/solvers.cpp \
    CALCUL/librairieAlglib/optimization.cpp \
    CALCUL/librairieAlglib/linalg.cpp \
    CALCUL/librairieAlglib/interpolation.cpp \
    CALCUL/librairieAlglib/integration.cpp \
    CALCUL/librairieAlglib/fasttransforms.cpp \
    CALCUL/librairieAlglib/diffequations.cpp \
    CALCUL/librairieAlglib/dataanalysis.cpp \
    CALCUL/librairieAlglib/alglibmisc.cpp \
    INTERFACE/staticgraph.cpp \
    INTERFACE/readingobject.cpp \
    INTERFACE/qcustomplot.cpp \
    INTERFACE/plotdata.cpp \
    INTERFACE/plotcluster.cpp \
    INTERFACE/gui_spikesorting.cpp \
    INTERFACE/clusteringobject.cpp \
    INTERFACE/clusteringcontroller.cpp \
    INTERFACE/tabhandlefiltering.cpp \
    CALCUL/test.cpp \
    CALCUL/clustering/msams_converge_core.cpp \
    CALCUL/clustering/landmarked_neighbors.cpp \
    INTERFACE/plotfitting.cpp \
    CALCUL/fonctionsutiles.cpp \
    INTERFACE/fonctionsutilesI.cpp

HEADERS  +=    CALCUL/enginesorting.h \
    CALCUL/filtering/filter.h \
    CALCUL/readdata.h \
    CALCUL/fonctionsutiles.h \
    CALCUL/librairieAlglib/ap.h \
    CALCUL/librairieAlglib/stdafx.h \
    CALCUL/librairieAlglib/statistics.h \
    CALCUL/librairieAlglib/specialfunctions.h \
    CALCUL/librairieAlglib/solvers.h \
    CALCUL/librairieAlglib/optimization.h \
    CALCUL/librairieAlglib/linalg.h \
    CALCUL/librairieAlglib/interpolation.h \
    CALCUL/librairieAlglib/integration.h \
    CALCUL/librairieAlglib/fasttransforms.h \
    CALCUL/librairieAlglib/diffequations.h \
    CALCUL/librairieAlglib/dataanalysis.h \
    CALCUL/librairieAlglib/alglibmisc.h \
    CALCUL/librairieAlglib/alglibinternal.h \
    INTERFACE/staticgraph.h \
    INTERFACE/readingobject.h \
    INTERFACE/qcustomplot.h \
    INTERFACE/plotfittingdata.h \
    INTERFACE/plotdata.h \
    INTERFACE/plotcluster.h \
    INTERFACE/gui_spikesorting.h \
    INTERFACE/clusteringobject.h \
    INTERFACE/clusteringcontroller.h \
    INTERFACE/tabhandlefiltering.h \
    CALCUL/test.h \
    CALCUL/clustering/msams_converge_core.h \
    CALCUL/clustering/landmarked_neighbors.h \
    INTERFACE/plotfitting.h \
    INTERFACE/fonctionsutilesI.h

FORMS    += INTERFACE/gui_spikesorting.ui
