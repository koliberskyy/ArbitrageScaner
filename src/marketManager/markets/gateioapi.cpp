#include "gateioapi.h"

GateIoApi::GateIoApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    :   MarketApi("https://api.gateio.ws",
        {"currency_pair", "last", "quote_volume"},
        __netManager,
        parent)
{

}
void GateIoApi::updateTokenList()
{
    sendGetRequest("/api/v4/spot/tickers", "", TAGS::TOKENLIST);
}

void GateIoApi::updatePriceMap()
{

}

void GateIoApi::updateVolumeMap()
{

}

void GateIoApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        set(QJsonDocument::fromJson(reply).array());
        emit parceComplete(info);
    }
}
