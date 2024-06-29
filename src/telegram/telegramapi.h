#ifndef TELEGRAMAPI_H
#define TELEGRAMAPI_H

#include <QObject>
#include "marketapi.h"

class TelegramApi : public QObject
{
    Q_OBJECT
public:
    explicit TelegramApi(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
    void sendMessage(const QString &message);
    void deleteLastMessage();
private:
    const QString host;
    std::shared_ptr<QNetworkAccessManager> netManager;
    int lastMessageId{0};
    void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply);

    void sendGetRequest(QNetworkRequest &&request);
    void sendGetRequest(const QString &method, const QString &query, const QString &userInfo = "");
    void sendGetRequest(QString &&method, QString &&query, QString &&userInfo = "");
private slots:
    void replyFinished();
signals:

};

#endif // TELEGRAMAPI_H
