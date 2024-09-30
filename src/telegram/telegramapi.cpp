#include "telegramapi.h"

TelegramApi::TelegramApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    : QObject{parent}, netManager{__netManager}, host{"https://api.telegram.org/bot6387989209:AAEwzRfOfEUGC0FsW5Mo1e9fvd0tV_xjkbI"}
{
    if(netManager == nullptr){
        netManager.reset(new QNetworkAccessManager(this));
    }
}

void TelegramApi::sendMessage(const QString &message)
{
    auto copy = message;
    sendGetRequest("/sendMessage?chat_id=615961766&text=", std::move(copy), "SNDMSG");
}

void TelegramApi::deleteLastMessage()
{
    if(lastMessageId != 0){
        auto tmp = QString::fromStdString(std::to_string(lastMessageId));
        sendGetRequest("/deleteMessage?chat_id=615961766&message_id=", std::move(tmp), "DELMSG");
    }

}

void TelegramApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto breakPoint = true;
    auto info = request.url().userInfo();
    if(info == "SNDMSG")
        lastMessageId = QJsonDocument::fromJson(reply).object()["result"].toObject()["message_id"].toInt();
    //else if(info == "DELMSG")
      //  deleteLastMessage();
}
void TelegramApi::sendGetRequest(QNetworkRequest &&request)
{
    auto reply = netManager->get(std::move(request));
    QObject::connect(reply, &QNetworkReply::finished,
                     this, &TelegramApi::replyFinished);
}

void TelegramApi::sendGetRequest(QString &&method, QString &&query, QString &&userInfo)
{
    QUrl url(host + std::move(method) + std::move(query));
    url.setUserInfo(std::move(userInfo));

    sendGetRequest(QNetworkRequest(std::move(url)));
}

void TelegramApi::sendGetRequest(const QString &method, const QString &query, const QString &userInfo)
{
    QUrl url(host + method + query);
    url.setUserInfo(userInfo);

    sendGetRequest(QNetworkRequest(std::move(url)));
}

void TelegramApi::replyFinished()
{
    auto reply = static_cast<QNetworkReply*>(sender());
    parceReply(reply->request(), reply->error(), reply->readAll());
    reply->deleteLater();
}
