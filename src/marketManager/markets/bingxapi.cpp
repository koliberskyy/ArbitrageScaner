#include "bingxapi.h"

BingXApi::BingXApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    :   MarketApi("https://open-api.bingx.com",
        {"symbol", "lastPrice", "quoteVolume"},
        __netManager,
        parent)
{

}
void BingXApi::updateTokenList()
{
    sendGetRequest("/openApi/swap/v2/quote/ticker", "", TAGS::TOKENLIST);

}

void BingXApi::updatePriceMap()
{

}

void BingXApi::updateVolumeMap()
{

}

void BingXApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        set(QJsonDocument::fromJson(reply).object()["data"].toArray());
        emit parceComplete(info);
    }
}
