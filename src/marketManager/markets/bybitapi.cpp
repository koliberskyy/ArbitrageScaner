#include "bybitapi.h"

BybitApi::BybitApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    : MarketApi("https://api.bybit.com",
                {"symbol", "lastPrice", "turnover24h", "bid1Price", "ask1Price", "bid1Size", "ask1Size"},
                __netManager,
                parent)
{

}

void BybitApi::updateTokenList()
{
#ifdef SPOT
    sendGetRequest("/v5/market/tickers", "?category=spot", TAGS::TOKENLIST);
#else
    sendGetRequest("/v5/market/tickers", "?category=linear", TAGS::TOKENLIST);
#endif
}

void BybitApi::updateCurrencies()
{

}

void BybitApi::updateVolumeMap()
{

}

void BybitApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        setTickers(QJsonDocument::fromJson(reply).object()["result"].toObject()["list"].toArray());
        emit parceComplete(info);
    }
}


