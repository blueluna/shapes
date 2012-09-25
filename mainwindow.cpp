#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFileOpen()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Hi");
    int result = shapefile.Open(filepath.toStdString());
    if (result != 0) {
        QMessageBox::warning(this, tr("Bad"), tr("Something is wrong"));
    }
    else {
        int count = shapefile.GetEntityCount();
        ui->shapeEntityCount->setText(QString::number(count));
        QString typeString = QString::fromStdString(shapefile.GetTypeString());
        ui->shapeType->setText(typeString);
        QList<QTreeWidgetItem *> items;
        for (int i = 0; i < count; i++) {
            QStringList list(QString::number(i));
            QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list);
            QVariant v(i);
            item->setData(0, Qt::UserRole, v);
            items.append(item);
        }
        ui->treeWidget->clear();
        ui->treeWidget->insertTopLevelItems(0, items);
        ShapeObject object;
        shapefile.GetShape(1, object);
        ui->glview->SetShape(object);
        ui->glview->updateGL();
    }
}

void MainWindow::onSelectShape(QTreeWidgetItem *item, int)
{
    if (item != 0) {
        bool ok;
        int index = item->data(0, Qt::UserRole).toInt(&ok);
        if (ok) {
            ShapeObject object;
            shapefile.GetShape(index, object);
            ui->glview->SetShape(object);
            ui->glview->updateGL();
        }
    }
}
