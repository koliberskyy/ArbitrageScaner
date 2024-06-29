#include "mexcapi.h"

MexcApi::MexcApi(std::shared_ptr<QNetworkAccessManager> __netManager,  QObject *parent)
    : MarketApi("https://api.mexc.com",
                {"symbol", "lastPrice", "quoteVolume"},
                __netManager,
                parent)
{
}
void MexcApi::updateTokenList()
{
    sendGetRequest("/api/v3/ticker/24hr","", TAGS::TOKENLIST);
}

void MexcApi::updatePriceMap()
{

}

void MexcApi::updateVolumeMap()
{

}

void MexcApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        set(QJsonDocument::fromJson(reply).array());
        emit parceComplete(info);
    }
}


