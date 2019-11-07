When it modified, You replace this line in Ui_MainWindow.h

	before: class Ui_MainWindow
	after: class Ui_MainWindow : public QMainWindow

more in RenderView.cpp

	before: static const uint qt_meta_data_RendererView[] = { 
	after: static const unsigned int qt_meta_data_RendererView[] = { 
	
	
Then program is leaved from complie error.