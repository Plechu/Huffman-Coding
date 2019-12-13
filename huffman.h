#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QMainWindow>

namespace Ui {
class Huffman;
}

class Huffman : public QMainWindow
{
    Q_OBJECT

public:
    explicit Huffman(QWidget *parent = nullptr);
    ~Huffman();

private slots:
    void on_top_browser_clicked();

    void on_bottom_browser_clicked();

    void on_run_button_clicked();

    void on_Code_clicked();

    void on_Decode_clicked();

private:
    Ui::Huffman *ui;
};

#endif // HUFFMAN_H
