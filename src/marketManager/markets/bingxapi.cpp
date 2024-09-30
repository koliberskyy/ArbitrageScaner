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
    sendGetRequest("/openApi/swap/v2/quote/ticker", "", TAGS::TOKENLIST);

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
