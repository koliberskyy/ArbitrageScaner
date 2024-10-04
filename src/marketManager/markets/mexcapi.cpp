#include "mexcapi.h"

MexcApi::MexcApi(std::shared_ptr<QNetworkAccessManager> __netManager,  QObject *parent)
#ifdef SPOT
    : MarketApi("https://api.mexc.com",
#else
    : MarketApi("https://contract.mexc.com",
#endif
                {"symbol", "lastPrice", "quoteVolume", "bidPrice", "askPrice", "bidQty", "askQty"},
                __netManager,
                parent)
{
}
void MexcApi::updateTokenList()
{
#ifdef SPOT
    sendGetRequest("/api/v3/ticker/24hr","", TAGS::TOKENLIST);
#else
    sendGetRequest("/api/v1/contract/ticker","", TAGS::TOKENLIST);
#endif

}

void MexcApi::updateCurrencies()
{

}

void MexcApi::updateVolumeMap()
{

}

void MexcApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
#ifdef SPOT
        setTickers(QJsonDocument::fromJson(reply).array());
#else
        setTickers(QJsonDocument::fromJson(reply).object()["data"].toArray());
#endif
        emit parceComplete(info);
    }
}


