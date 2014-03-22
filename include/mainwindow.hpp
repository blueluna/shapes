#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "shapefile.hpp"

namespace Ui {
class MainWindow;
}

using namespace coldstar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
	ShapeFile shapefile;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

private slots:
    void onFileOpen();
    void onSelectShape(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_HPP
