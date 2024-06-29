#include "marketmanager.h"

MarketManager::MarketManager(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    : QObject{parent}, netManager{__netManager}
{
    QObject::connect(this, &MarketManager::tokenUpdatingComplete, this, &MarketManager::findSpred);


    if(netManager == nullptr){
        netManager.reset(new QNetworkAccessManager(this));
    }

    markets.reserve(4);
    markets.emplace_back(new BybitApi(netManager));
    markets.emplace_back(new MexcApi(netManager));
    markets.emplace_back(new BingXApi(netManager));
    markets.emplace_back(new GateIoApi(netManager));
}

void MarketManager::updateTokens()
{

    updateCounter = 0;
    for(auto &it : markets){
        QObject::connect(it.get(), &MarketApi::parceComplete, this, &MarketManager::checkUpdate);
        it->updateTokenList();
    }

}

void MarketManager::checkUpdate()
{
    updateCounter_mtx.lock();
    ++updateCounter;
    if(updateCounter == markets.size()){
        emit tokenUpdatingComplete();
        //updateCounter = 0;
    }
    updateCounter_mtx.unlock();
}

void MarketManager::findSpred()
{
    if(markets.size() > 1){
        std::vector<Spred> reply;
        auto curr = markets.begin();
        auto checkable = curr;

        while (curr != markets.end()){
            checkable = curr;
            ++checkable;
            while(checkable != markets.end()){
                if(curr != checkable){
                    auto crossTokens = MarketApi::getCrossTokens((*curr)->getAvaibleTokens(), (*checkable)->getAvaibleTokens());
                    auto spredVec = MarketApi::getSpredVec(std::move(crossTokens), *curr, *checkable);
                    for (auto &it : spredVec){
                        reply.emplace_back(it);
                    }
                }
                ++checkable;
            }
            ++curr;
        }

        if(!reply.empty()){
            std::sort(reply.begin(), reply.end(), [](Spred s1, Spred s2){return s1.spred > s2.spred;});

            auto it = reply.begin();

            while(it->spred > 6.0 && it != reply.end()){
                ++it;
            }
            reply.erase(reply.begin(), it);

            it = reply.begin();

            while(it->spred > 2.0 && it != reply.end()){
                ++it;
            }

            reply.erase(it, reply.end());


            QFile file("spred.txt");
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            for(auto &it : reply){
                file.write(it.toQString().toUtf8());
            }
            file.close();
            std::cout << "done" << QDateTime::currentDateTime().toString(" dd.MM-hh:mm.ss").toStdString() <<"\n";

            spredVec = std::move(reply);
            emit spredUpdated();
        }

    }
}
