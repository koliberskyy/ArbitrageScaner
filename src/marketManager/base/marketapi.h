#ifndef MARKETAPI_ABS_H
#define MARKETAPI_ABS_H

#include <QObject>
#include <QList>
#include <QString>
#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>
#include <iostream>
#include <array>
#include <list>
#include <vector>


using value_type = double;
using key_type = QString;

struct JsonArgNames{
    const key_type symbol{"symbol"};
    const key_type lastPrice{"lastPrice"};
    const key_type volume{"volume"};
};

struct TAGS {
        inline static const key_type TOKENLIST{"TOKENLIST_TAG"};
};

struct Spred{
    explicit Spred(const QString &market1, double price1, const QString &market2, double price2, const QString &token, double volume1, double volume2){
        if(price1 < price2){
            market_from = market1;
            price_from = price1;

            market_to = market2;
            price_to = price2;

            volume_from = volume1;
            volume_to = volume2;
        }
        else{
            market_from = market2;
            price_from = price2;

            market_to = market1;
            price_to = price1;

            volume_from = volume2;
            volume_to = volume1;
        }
        setSpred();

        symbol = token;
    }
    void setSpred(){
        if(price_from > 0 && price_to > 0)
            spred = 100 * (price_to - price_from)/price_from;
        else
            spred = -1;
    }
    static QString getMarketNaame(QString marketHost){
        marketHost.remove("https://api.");
        marketHost.remove(".com");
        marketHost.remove("https://open-api.");
        marketHost.remove(".ws");

        return marketHost;
    }
    QString market_from;
    QString market_to;
    double price_from;
    double price_to;
    double spred;
    QString symbol;

    double volume_from;
    double volume_to;

    auto toQString()const {return QString(symbol +": " + getMarketNaame(market_from) + "->" + getMarketNaame(market_to) + "\n" +
                                          QString::fromStdString(std::to_string(price_from)) + "->" + QString::fromStdString(std::to_string(price_to)) + "\n" +
                                          QString::fromStdString(std::to_string(volume_from)) + "->" + QString::fromStdString(std::to_string(volume_to)) + "\n" +
                                            "spred: " + QString::fromStdString(std::to_string(spred)) +
                                            "\n----------------------\n");}
};

/*!
 * Класс содержащий интерфесы работы с api бирж
*/
class MarketApi : public QObject
{
    Q_OBJECT
public:
    explicit MarketApi(const QString &__host, JsonArgNames &&__argNames = JsonArgNames(), std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);

    auto getAvaibleTokens() const
    {
        return tokenList;
    }
    auto getPriceMap() const
    {
        return &priceMap;
    }
    auto getVolumeMap() const
    {
        return &volumeMap;
    }
    auto isReplyFinished() const
    {
        return updated;
    }

    static std::list<key_type> getCrossTokens(std::list<key_type> first, std::list<key_type> second);
    static std::vector<Spred> getSpredVec(std::list<key_type> &&tokens, std::shared_ptr<MarketApi> market1, std::shared_ptr<MarketApi> market2);

public slots:
    virtual void updateTokenList() = 0;
    virtual void updatePriceMap() = 0;
    virtual void updateVolumeMap() = 0;

protected:
    std::list<key_type> tokenList;              //список токенов, торуемых на бирже
    QHash<key_type, value_type> priceMap;   //хеш-мап пар токен-рыночная цена
    QHash<key_type, value_type> volumeMap;  //хеш-мап пар токен-объем торгов за сутки
    const QString baseToken = "USDT";
    const double minVolume = 100'000.0;
    const double maxVolume = 1'000'000'000'000.0;

    const QString host;
    std::shared_ptr<QNetworkAccessManager> netManager;
    virtual void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply) = 0;

    void sendGetRequest(QNetworkRequest &&request);
    void sendGetRequest(const QString &method, const QString &query, const QString &userInfo = "");
    void sendGetRequest(QString &&method, QString &&query, QString &&userInfo = "");

    const JsonArgNames jsonArgNames;
    void set(QJsonArray &&tokens);
    void setUpdated(bool trg = true){updated = true;}

protected slots:
    void replyFinished();

signals:
    void parceComplete(key_type TAG);

private:
    bool updated{false};

    template<typename StringType>
    void symbolFilter(StringType *symbol){
        for(auto it = symbol->begin(); it != symbol->end(); ++it){
            if(*it == '-' || *it == '_'){
                auto next = it + 1;
                symbol->erase(it, next);
            }
        }
    }

};

#endif // MARKETAPI_ABS_H
