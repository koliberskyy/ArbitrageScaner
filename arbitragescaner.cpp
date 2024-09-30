#include "arbitragescaner.h"

ArbitrageScaner::ArbitrageScaner(QObject *parent)
    : QObject{parent},
      netManager{new QNetworkAccessManager()},
      markManager{new MarketManager(netManager)},
      timer{new QTimer(this)},
      tg{new TelegramApi(netManager)},
      spredUpdateFluencySec(new qint64(3)),
      spredUpdateTime(new QDateTime(QDateTime::currentDateTime()))
{
    QObject::connect(markManager.get(), &MarketManager::spredUpdated, this, &ArbitrageScaner::printResult);
    QObject::connect(markManager.get(), &MarketManager::spredUpdated, this, &ArbitrageScaner::sendResult);
    QObject::connect(timer.get(), &QTimer::timeout, this, &ArbitrageScaner::timerChanged);
    scan();
    timer->start();
}

void ArbitrageScaner::scan()
{
    markManager->updateTokens();
}

void ArbitrageScaner::sendResult()
{
    auto spred = markManager->getSpredVec();
    if(!spred->empty()){
        QString message;
        for(auto &it : *spred){
            message.append(it.toQString());
        }
        tg->deleteLastMessage();

        tg->sendMessage(message);
    }
}

void ArbitrageScaner::printResult()
{
    auto spred = markManager->getSpredVec();

    if(!spred->empty()){

        std::cout << "\x1B[2J\x1B[H";
        QString message(QDateTime::currentDateTime().toString("hh:mm:ss - dd.MM.yyyy"));
        message.append("\n\n");
        for(auto &it : *spred){
            message.append(it.toQString());
        }

        std::cout << message.toStdString();
    }
}

void ArbitrageScaner::timerChanged()
{
    auto current = QDateTime::currentDateTime();
    //update pos
    if(spredUpdateTime->secsTo(current) > *spredUpdateFluencySec){
        spredUpdateTime->setDate(current.date());
        spredUpdateTime->setTime(current.time());
        scan();
    }
}
