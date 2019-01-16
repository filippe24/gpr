#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"


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

void MainWindow::on_action_Quit_triggered()
{
	 QApplication::quit();
}

void MainWindow::on_checkBoxFill_toggled(bool checked)
{
	 ui->openGLWidget->setPolygonMode(checked);
}

void MainWindow::on_action_Open_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open PLY"), ".", tr("*.ply"));

	ui->openGLWidget->loadMesh(filename);
}


//MY FUNCTIONS
void MainWindow::setDefaultMesh()
{
    ui->openGLWidget->accesMesh().setDefault();
    ui->openGLWidget->makeCurrent();
    ui->openGLWidget->accesMesh().buildMesh();
    ui->openGLWidget->doneCurrent();
    ui->openGLWidget->update();
}





//MY SLOTS

void MainWindow::on_Mean_clicked(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->set_selected_gauss(false);
    }
    else ui->openGLWidget->set_selected_gauss(true);


}

void MainWindow::on_Gaussian_clicked(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->set_selected_gauss(true);
    }
    else ui->openGLWidget->set_selected_gauss(false);
}

void MainWindow::on_checkBoxRefl_clicked(bool checked)
{
    ui->openGLWidget->set_reflection_lines(checked);
}


void MainWindow::on_ComputeCurvature_clicked()
{
    ui->openGLWidget->compute_curvatures();
}


//~~~~~~~~~LAPLACIAN~~BUTTONS~~~~~~~~~~~~~
void MainWindow::on_LaplacianPush_clicked()
{
    ui->openGLWidget->laplacian_operator(false);
}
void MainWindow::on_LaplacianCotgPush_clicked()
{
    ui->openGLWidget->laplacian_operator(true);
}
void MainWindow::on_iterativeSelection_activated(int index)
{
    ui->openGLWidget->accesMesh().setCurrentIterativeType(index);
}

//~~~~~~~~~LAMBDA~DEFINITION~~BUTTONS~~~~~~~~~~~~~
void MainWindow::on_lambdaSlider_actionTriggered(int action)
{
    ui->openGLWidget->accesMesh().setLambda(float(action/100.0f));
}

//~~~~~~~~~DEFAULT~~BUTTONS~~~~~~~~~~~~~
void MainWindow::on_defaultButton_clicked()
{
    setDefaultMesh();
}

void MainWindow::on_defaultButton_2_clicked()
{
    setDefaultMesh();
}


//~~~~~~~~~GLOBAL~SMOOTH~~~~~~~~~~~~~
void MainWindow::on_globalSmoothPush_clicked()
{
    //uniform
    ui->openGLWidget->global_smoothing(false);
}
void MainWindow::on_globalSmoothCotangPush_clicked()
{
    //cotangent
    ui->openGLWidget->global_smoothing(true);
}
//set percentage
void MainWindow::on_percentageSpinBox_valueChanged(int arg1)
{
    ui->openGLWidget->accesMesh().setPercentage(arg1);
}

//~~~~~~~~~CURVATURE~COLOR~~~~~~~~~
void MainWindow::on_gaussianWeight_sliderMoved(int position)
{
    ui->openGLWidget->gauss_weight = float(position)/100.0f;
    ui->openGLWidget->update();
}
void MainWindow::on_meanWeight_sliderMoved(int position)
{
    ui->openGLWidget->mean_weight = float(position/100.0f);
    ui->openGLWidget->update();

}

//~~~~~~~~~PARAMETRIZE~~~~~~~~~~~~~
void MainWindow::on_parametrizeButton_clicked()
{
    ui->openGLWidget->accesMesh().computeParametrization();
}



//~~~~~~~~~MAGNIFY~~~~~~~~~~~~~
void MainWindow::on_magnifyUniform_clicked()
{
    //uniform
    ui->openGLWidget->magnify_details(false);
}

void MainWindow::on_magnifyCotangent_clicked()
{
    //uniform
    ui->openGLWidget->magnify_details(true);
}

void MainWindow::on_iteration1Spin_valueChanged(int arg1)
{
    ui->openGLWidget->accesMesh().setNumIterationMag(arg1);
}

void MainWindow::on_iteration2Spin_valueChanged(int arg1)
{
    ui->openGLWidget->accesMesh().setMultiplierMag(arg1);

}

void MainWindow::on_lambdaMagSpin_valueChanged(double arg1)
{
    ui->openGLWidget->accesMesh().setParameterMag(arg1);
}
