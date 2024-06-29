#include "bybitapi.h"

BybitApi::BybitApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    : MarketApi("https://api.bybit.com",
                {"symbol", "lastPrice", "turnover24h"},
                __netManager,
                parent)
{

}

void BybitApi::updateTokenList()
{
    sendGetRequest("/v5/market/tickers", "?category=spot", TAGS::TOKENLIST);
}

void BybitApi::updatePriceMap()
{

}

void BybitApi::updateVolumeMap()
{

}

void BybitApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        set(QJsonDocument::fromJson(reply).object()["result"].toObject()["list"].toArray());
        emit parceComplete(info);
    }
}


