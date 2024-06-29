#include "arbitragescaner.h"

ArbitrageScaner::ArbitrageScaner(QObject *parent)
    : QObject{parent},
      netManager{new QNetworkAccessManager()},
      markManager{new MarketManager(netManager)},
      timer{new QTimer(this)},
      tg{new TelegramApi(netManager)},
      spredUpdateFluencySec(new qint64(60)),
      spredUpdateTime(new QDateTime(QDateTime::currentDateTime()))
{
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
        QString message("\n\n BEGIN *****************************\n");
        for(auto &it : *spred){
            message.append(it.toQString());
        }

        tg->sendMessage(message);
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
