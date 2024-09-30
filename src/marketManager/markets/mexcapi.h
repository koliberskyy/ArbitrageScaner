#ifndef MEXCAPI_H
#define MEXCAPI_H

#include <marketapi.h>


class MexcApi : public MarketApi
{
    Q_OBJECT
public:
    explicit MexcApi(std::shared_ptr<QNetworkAccessManager> __netManager = nullptr, QObject *parent = nullptr);
public slots:
    virtual void updateTokenList() final;
    virtual void updateCurrencies() final;
    virtual void updateVolumeMap() final;
protected:
    virtual void parceReply(QNetworkRequest &&request, QNetworkReply::NetworkError &&error, QByteArray &&reply) final;

};

#endif // MEXCAPI_H
