#include "Memo.h"
#include "ui_Memo.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QShortcut>
#include <QKeySequence>

Memo::Memo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Memo)
{
    ui->setupUi(this);

    //컨트롤S 눌러서 저장
    QShortcut* saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(saveShortcut, &QShortcut::activated,
            this, &Memo::saveMemo);

    loadMemo();
}

Memo::~Memo()
{
    //종료시 자동 저장
    saveMemo();
    delete ui;
}

QString Memo::memoFilePath() const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(basePath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.filePath("memo.json");
}

void Memo::saveMemo()
{
    QFile file(memoFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    //메모장 여러개 추가시 구분하기 위해서 obj추가 당장은 1장의 메모만 존재
    QJsonObject obj;
    obj["content"] = ui->memo_TextEdit->toPlainText();
    obj["updated_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

}

void Memo::loadMemo()
{
    QFile file(memoFilePath());

    if (!file.exists()) {
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return;
    }

    QJsonObject obj = doc.object();
    ui->memo_TextEdit->setPlainText(obj.value("content").toString());
}
