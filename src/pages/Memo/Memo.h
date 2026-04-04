#ifndef MEMO_H
#define MEMO_H

#include <QWidget>

namespace Ui {
class Memo;
}

class Memo : public QWidget
{
    Q_OBJECT

public:
    explicit Memo(QWidget *parent = nullptr);
    ~Memo();

private:
    Ui::Memo *ui;

    void saveMemo();
    void loadMemo();
    QString memoFilePath() const;
};

#endif // MEMO_H
