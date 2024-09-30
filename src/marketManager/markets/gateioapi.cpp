#include "gateioapi.h"

GateIoApi::GateIoApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    :   MarketApi("https://api.gateio.ws",
        {"currency_pair", "last", "quote_volume", "highest_bid", "lowest_ask", "highest_size", "lowest_size"},
        __netManager,
        parent)
{

}
void GateIoApi::updateTokenList()
{
    sendGetRequest("/api/v4/spot/tickers", "", TAGS::TOKENLIST);
}

void GateIoApi::updateCurrencies()
{

}

void GateIoApi::updateVolumeMap()
{

}

void GateIoApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        setTickers(QJsonDocument::fromJson(reply).array());
        emit parceComplete(info);
    }
}
