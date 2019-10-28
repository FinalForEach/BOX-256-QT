#include "tablecelledit.h"

TableCellEdit::TableCellEdit(QWidget *parent, int cellRow, int cellColumn)
    :QPlainTextEdit(parent), i(cellRow), j(cellColumn)
{
}
TableCellEdit::~TableCellEdit()
{
}
