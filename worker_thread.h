#pragma once
#include <QThread>
#include <QString>
#include <atomic>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(const QString& filename, QObject* parent = nullptr);

    // Метод для безпечної зупинки ззовні
    void requestStop();

protected:
    // Цей метод виконується в новому потоці
    void run() override;

signals:
    void progressChanged(int percent);
    void finished(const QString& result);
    void errorOccurred(const QString& error);

private:
    QString m_filename;
    // atomic<bool> — найпростіший спосіб комунікації між потоками.
    // std::atomic гарантує атомарне читання/запис без mutex.
    std::atomic<bool> m_stopRequested{false};
};
