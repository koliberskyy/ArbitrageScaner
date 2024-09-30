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

        //linear scan
        while (curr != markets.end()){
            checkable = curr;
            ++checkable;
            while(checkable != markets.end()){
                if(curr != checkable){
                    auto crossTokens = MarketApi::getCrossTokens((*curr)->getAvaibleTokens(), (*checkable)->getAvaibleTokens());
                    auto spred = MarketApi::getSpredVec(std::move(crossTokens), *curr, *checkable, spredVec);
                    for (auto &it : spred){
                        reply.emplace_back(it);
                    }
                }
                ++checkable;
            }
            ++curr;
        }

        //reverse scan
        auto rcurr = markets.rbegin();
        auto rcheckable = rcurr;
        while (rcurr != markets.rend()){
            rcheckable = rcurr;
            ++rcheckable;
            while(rcheckable != markets.rend()){
                if(rcurr != rcheckable){
                    auto crossTokens = MarketApi::getCrossTokens((*rcurr)->getAvaibleTokens(), (*rcheckable)->getAvaibleTokens());
                    auto spred = MarketApi::getSpredVec(std::move(crossTokens), *rcurr, *rcheckable, spredVec);
                    for (auto &it : spred){
                        reply.emplace_back(it);
                    }
                }
                ++rcheckable;
            }
            ++rcurr;
        }

        if(!reply.empty()){
            std::sort(reply.begin(), reply.end(), [](Spred s1, Spred s2){return s1.spred < s2.spred;});

            auto it = reply.begin();

            while(it->spred < 1.0 && it != reply.end()){
                ++it;
            }
            reply.erase(reply.begin(), it);

            it = reply.begin();

            while(it->spred < 100.0 && it != reply.end()){
                ++it;
            }

            reply.erase(it, reply.end());


            // QFile file("spred.txt");
            // file.open(QIODevice::WriteOnly | QIODevice::Text);
            // for(auto &it : reply){
            //     file.write(it.toQString().toUtf8());
            // }
            // file.close();
            // std::cout << "done" << QDateTime::currentDateTime().toString(" dd.MM-hh:mm.ss").toStdString() <<"\n";

            std::sort(reply.begin(), reply.end(), [](Spred s1, Spred s2){return s1.confimCount < s2.confimCount;});


            spredVec = std::move(reply);
            emit spredUpdated();
        }

    }
}
