#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "shapefile.h"

namespace Ui {
class MainWindow;
}

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
};

#endif // MAINWINDOW_H
