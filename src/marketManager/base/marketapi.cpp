#include "marketapi.h"

MarketApi::MarketApi(const QString &__host, JsonArgNames &&__argNames, std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    : QObject{parent}, netManager{__netManager}, host{__host}, jsonArgNames{std::move(__argNames)}
{
    if(netManager == nullptr){
        netManager.reset(new QNetworkAccessManager(this));
    }
}

std::list<key_type> MarketApi::getCrossTokens(std::list<key_type> first, std::list<key_type> second)
{
    first.merge(std::move(second));

    auto curr = first.begin();
    auto next = ++first.begin();

    while(next != first.end()){
        if(*curr != *next){
            curr = first.erase(curr);
            ++next;
        }
        else{
            ++curr;
            ++next;
        }
    }

    return std::move(first);
}

std::vector<Spred> MarketApi::getSpredVec(std::list<key_type> &&tokens, std::shared_ptr<MarketApi> market1, std::shared_ptr<MarketApi> market2, const std::vector<Spred> &prev)
{
    std::vector<Spred> reply;

    for(auto &it:tokens){
        auto price1 = market1->getAskMap()->value(it, -1);
        auto price2 = market2->getBidMap()->value(it, -1);
        if(price1 > 0 && price2 > 0 && price1 < price2){
            reply.emplace_back(market1->host, price1, market2->host, price2, it,
                               market1->getAskQtyMap()->value(it,-1),
                               market2->getBidQtyMap()->value(it,-1));
        }
    }
    for(auto &new_it : reply){
        for (const auto &old_it : prev){
            if(new_it == old_it){
                new_it.confimCount = old_it.confimCount + 1;
                new_it.beginTime = old_it.beginTime;
            }
        }
    }

    for(auto &it : reply){
        if(it.confimCount == 0)
            it.beginTime = QDateTime::currentDateTime();
    }

    return reply;
}

void MarketApi::sendGetRequest(const QString &method, const QString &query, const QString &userInfo)
{
    QUrl url(host + method + query);
    url.setUserInfo(userInfo);

    sendGetRequest(QNetworkRequest(std::move(url)));
}

void MarketApi::sendGetRequest(QString &&method, QString &&query, QString &&userInfo)
{
    QUrl url(host + std::move(method) + std::move(query));
    url.setUserInfo(std::move(userInfo));

    sendGetRequest(QNetworkRequest(std::move(url)));
}

void MarketApi::setTickers(QJsonArray &&tokens)
{
    tokenList.clear();
    volumeMap.clear();
    priceMap.clear();

    for(auto it = tokens.begin(); it != tokens.end(); it++){
        auto obj = it->toObject();
        auto symbol = obj[jsonArgNames.symbol].toString();
        symbolFilter(&symbol);
        auto volume = obj[jsonArgNames.volume].toString().toDouble();


        if(symbol.endsWith(baseToken)
                && volume > minVolume
                && volume < maxVolume){

            tokenList.emplace_back(symbol);

            volumeMap.emplace(symbol, volume);

            auto bid =  obj[jsonArgNames.bid].toString().toDouble();
            bidMap.emplace(symbol, bid);

            auto ask = obj[jsonArgNames.ask].toString().toDouble();
            askMap.emplace(symbol, ask);

            auto bidQty = obj[jsonArgNames.bidQty].toString();
            if(!bidQty.isEmpty())
                bidQtyMap.emplace(symbol, bidQty.toDouble() * bid);
            else
                bidQtyMap.emplace(symbol, 0);

            auto askQty = obj[jsonArgNames.askQty].toString();
            if(!askQty.isEmpty())
                askQtyMap.emplace(symbol, askQty.toDouble() * ask);
            else
                askQtyMap.emplace(symbol, 0);

            priceMap.emplace(std::move(symbol), obj[jsonArgNames.lastPrice].toString().toDouble());
        }
    }
    if(!tokenList.empty()){
        tokenList.sort();
    }

    setUpdated();
}

void MarketApi::setCurrencies(QJsonArray &&currencies)
{
    for(const auto &it : currencies){
        auto obj = it.toObject();
        auto symbol = obj[jsonArgNames.symbol].toString();
        symbolFilter(&symbol);
        if(symbol.endsWith(baseToken)){
            currenciesMap.emplace(std::move(symbol), obj);
        }
    }
}

void MarketApi::sendGetRequest(QNetworkRequest &&request)
{
    setUpdated(false);
    auto reply = netManager->get(std::move(request));
    QObject::connect(reply, &QNetworkReply::finished,
                     this, &MarketApi::replyFinished);
}

void MarketApi::replyFinished()
{
    auto reply = static_cast<QNetworkReply*>(sender());
    parceReply(reply->request(), reply->error(), reply->readAll());
    reply->deleteLater();
}


