#include "huffman.h"
#include "ui_huffman.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QTextEdit>
#include <QFileInfo>
#include <vector>
#include "functions.h"

Huffman::Huffman(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Huffman){
    ui->setupUi(this);
    this->setFixedSize(QSize(260, 300)); // uniemozliwienie skalowania
    QPixmap logo("logo.png"); // wczytanie logo
    ui->label_logo->setPixmap(logo); // umieszczenie logo w label_logo
    ui->label_logo->setScaledContents(true); // autoskalowanie loga
    ui->Code->setChecked(true); // domyslne zaznaczenie code
    ui->information_label->setAlignment(Qt::AlignCenter); // wysrodkowany text w information_label
    ui->presentation_label->setAlignment(Qt::AlignCenter);
    ui->top_file->setReadOnly(true); // pole z sciezka pliku tylko do wybrania za pomoca browse
    ui->bottom_file->setReadOnly(true);
}

Huffman::~Huffman(){
    delete ui;
}

void Huffman::on_top_browser_clicked(){
    QString file_name;
    QString filter; // filtr
    if(ui->Code->isChecked()){
        filter = "Choose file (*.*)"; // narazie dla bezpieczenstwa
        file_name = QFileDialog::getOpenFileName(this,"","",filter);
        ui->top_file->setText(file_name);
    }

    if(ui->Decode->isChecked()){
        filter = "Binary file (*.bin)"; // tylko plik binarny
        file_name = QFileDialog::getOpenFileName(this,"","",filter);
        ui->top_file->setText(file_name);
    }
}

void Huffman::on_bottom_browser_clicked(){
    QString file_name;
    QString filter; // filtr
    if(ui->Code->isChecked()){
        filter = "Binary file (*.bin)";
        file_name = QFileDialog::getSaveFileName(this,"","",filter);
        ui->bottom_file->setText(file_name);
    }

    if(ui->Decode->isChecked()){
        filter = "Choose file (*.*)";
        file_name = QFileDialog::getSaveFileName(this,"","",filter);
        ui->bottom_file->setText(file_name);
    }
}

void Huffman::on_run_button_clicked(){
    ui->information_label->setText(""); // wyczyszczenie information_label
    std::vector<Sign> collection(256); // kolekcja znakow, 256 to maksymalny rozmiar
    QString top_file_name = ui->top_file->displayText(); // sciezka wybrana w gornym browsie
    QString bottom_file_name = ui->bottom_file->displayText(); // sciezka wybrana w dolnym browsie

    if(ui->Code->isChecked()){ // jesli zaznaczono kodowanie
        try{ code(top_file_name.toStdString(), bottom_file_name.toStdString(), collection); } // kodowanie
        catch (int err_code) {
            if (err_code == -1) {
                ui->information_label->setStyleSheet("QLabel {color : red; }");
                ui->information_label->setText("File error!");
                return;
            }
        }
    }

    if(ui->Decode->isChecked()){ // jesli zaznaczono dekodowanie
        try { decode(top_file_name.toStdString(), bottom_file_name.toStdString(), collection); } // dekodowanie
        catch (int code) {
            if (code == -1) {
                ui->information_label->setStyleSheet("QLabel {color : red; }");
                ui->information_label->setText("File error!");
                return;
            }
        }
    }

    QFileInfo check_file(bottom_file_name);
      if (check_file.exists() && check_file.isFile()){ // sprawdzanie czy plik istnieje
          ui->information_label->setStyleSheet("QLabel {color : green; }");
          ui->information_label->setText("Success!");
      }else{
          ui->information_label->setStyleSheet("QLabel {color : red; }");
          ui->information_label->setText("Failure!");
      }

      ui->top_file->setText("Select path..."); // resetowanie sciezki dla bezpieczenstwa
      ui->bottom_file->setText("Select path...");
}

void Huffman::on_Code_clicked(){
    ui->top_file->setText("Select path..."); // dla bezpieczenstwa
    ui->bottom_file->setText("Select path...");
}

void Huffman::on_Decode_clicked(){
    ui->top_file->setText("Select path...");
    ui->bottom_file->setText("Select path...");
}

