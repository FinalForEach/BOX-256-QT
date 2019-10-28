#ifndef TABLECELLEDIT_H
#define TABLECELLEDIT_H

#include <QPlainTextEdit>

class TableCellEdit : public QPlainTextEdit
{
private:
    int i,j;
public:
    TableCellEdit(QWidget *parent, int cellRow, int cellColumn);
    ~TableCellEdit() override;
    int getCellRow() const{return i;}
    int getCellColumn() const{return j;}
};

#endif // TABLECELLEDIT_H
