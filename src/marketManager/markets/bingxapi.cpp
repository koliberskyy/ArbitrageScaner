#include "bingxapi.h"

BingXApi::BingXApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    :   MarketApi("https://open-api.bingx.com",
        {"symbol", "lastPrice", "quoteVolume", "bidPrice", "askPrice", "bidQty", "askQty"},
        __netManager,
        parent)
{

}
void BingXApi::updateTokenList()
{
#ifdef SPOT
    sendGetRequest("/openApi/spot/v1/ticker/24hr", "", TAGS::TOKENLIST);
#else
    sendGetRequest("/openApi/swap/v2/quote/ticker", "", TAGS::TOKENLIST);
#endif
}

void BingXApi::updateCurrencies()
{

}

void BingXApi::updateVolumeMap()
{

}

void BingXApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        setTickers(QJsonDocument::fromJson(reply).object()["data"].toArray());
        emit parceComplete(info);
    }
}
