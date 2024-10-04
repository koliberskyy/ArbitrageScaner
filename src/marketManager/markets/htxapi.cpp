#include "htxapi.h"

HtxApi::HtxApi(std::shared_ptr<QNetworkAccessManager> __netManager, QObject *parent)
    : MarketApi("https://api.huobi.pro",
                {"symbol", "close", "vol", "bid", "ask", "bidSize", "askSize"},
                __netManager,
                parent)
{

}

void HtxApi::updateTokenList()
{
#ifdef SPOT
    sendGetRequest("/market/tickers", "", TAGS::TOKENLIST);
#else
#endif
}

void HtxApi::updateCurrencies()
{

}

void HtxApi::updateVolumeMap()
{

}

void HtxApi::parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply)
{
    auto info = request.url().userInfo();

    if(info == TAGS::TOKENLIST){
        setTickers(QJsonDocument::fromJson(reply).object()["data"].toArray());
        emit parceComplete(info);
    }
}


