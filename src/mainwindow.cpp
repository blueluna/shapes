#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "poly2tri_helpers.hpp"

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
		ShapeObject object;
		for (int i = 0; i < count; i++) {
			QStringList list(QString::number(i));
			QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list);
			QVariant v(i);
			item->setData(0, Qt::UserRole, v);
			items.append(item);
			shapefile.GetShape(i, object);
			ui->glview->AddShape(object);
		}
		ui->treeWidget->clear();
		ui->treeWidget->insertTopLevelItems(0, items);
		ui->glview->Zoom(-1);
		ui->glview->updateGL();
		uint32_t vCount = ui->glview->GetVertexCount();
		ui->vertexCount->setText(QString::number(vCount));
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

			qDebug() << "Shape " << index;
			int32_t vertexCount = object.GetVertexCount();

			QFile vertexFile("vertex.txt");
			vertexFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
			{
				QTextStream vstrm(&vertexFile);
				const Point<double> *p_ptr = object.GetVertices();
				const Point<double> *p_end = p_ptr + vertexCount;
				while (p_ptr < p_end) {
					vstrm << p_ptr->x << ", " << p_ptr->y << "\n";
					p_ptr++;
				}
				p_ptr = object.GetVertices();
				double x_first = p_ptr->x;
				double y_first = p_ptr->y;
				p_ptr += (vertexCount - 1);
				double x_last = p_ptr->x;
				double y_last = p_ptr->y;
				if (x_first == x_last && y_first == y_last) {
					qDebug() << "First == Last";
					vertexCount--;
				}
			}
			vertexFile.close();

			QFile triangleFile("triangle.txt");
			triangleFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

			std::vector<p2t::Point*> polyline = CreatePolyLine(object.GetVertices(), vertexCount);
			try {
				QTextStream tstrm(&triangleFile);
				p2t::CDT cdt(polyline);
				cdt.Triangulate();
				std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();
				p2t::Point* pt = 0;
				for (auto ptri : triangles) {
					for (int i = 0; i < 3; i++) {
						pt = ptri->GetPoint(i);
						tstrm << pt->x << ", " << pt->y << "\n";
					}
				}
				ui->glview->SetPolygon(triangles);
			}
			catch (...) {}
			DestroyPolyLine(polyline);

			triangleFile.close();

			ui->glview->Zoom(index);
			ui->glview->updateGL();
		}
	}
}
