#ifndef ARBITRAGESCANER_H
#define ARBITRAGESCANER_H

#include <QObject>
#include <QTimer>
#include "marketmanager.h"
#include "telegramapi.h"

/*!
 * Класс сканирующий цены токенов с различных бирж, с целью определния спреда, образованного разницей цен одного точена на разных биржах
*/
class ArbitrageScaner : public QObject
{
    Q_OBJECT
public:
    explicit ArbitrageScaner(QObject *parent = nullptr);

private:
    std::shared_ptr<QNetworkAccessManager> netManager;
    std::shared_ptr<MarketManager> markManager;
    std::shared_ptr<TelegramApi> tg;

    std::unique_ptr<QTimer> timer;

    std::unique_ptr<QDateTime> spredUpdateTime;
    std::unique_ptr<qint64> spredUpdateFluencySec;

private slots:
    void scan();
    void sendResult();
    void printResult();
    void timerChanged();
signals:

};

#endif // ARBITRAGESCANER_H
