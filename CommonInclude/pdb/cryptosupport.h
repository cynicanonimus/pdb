#ifndef CRYPTOSUPPORT_H
#define CRYPTOSUPPORT_H
/*
Wrapper for OCA calls
*/
#include <QObject>
#include <QtCrypto/QtCrypto>

class CryptoSupport : public QObject
{
    Q_OBJECT
public:
    explicit CryptoSupport(QObject *parent = 0);
public:
    enum SymmetricMethods
    {
        NO_ENCRYPT          = 0,
        //AES128_ECB          = 1,    //!
        AES128_CFB          = 1,
        AES128_CBC          = 2,
        AES128_CBC_PKCS7    = 3,
        AES128_OFB          = 4,
        //AES192_ECB          = 6,    //!
        AES192_CFB          = 5,
        AES192_CBC          = 6,
        AES192_CBC_PKCS7    = 7,
        AES192_OFB          = 8,
        //AES256_ECB          = 11,   //!
        AES256_CBC          = 9,
        AES256_CBC_PKCS7    = 10,
        AES256_CFB          = 11,
        AES256_OFB          = 12,
        //BLOWFISH_ECB        = 16,   //!
        BLOWFISH_CBC_PKCS7  = 13,
        BLOWFISH_CBC        = 14,
        BLOWFISH_CFB        = 15,
        BLOWFISH_OFB        = 16,
        //TRIPLEDES_ECB       = 21,   //!
        TRIPLEDES_CBC       = 17,
        //DES_ECB             = 23,   //!
        //DES_ECB_PKCS7       = 24,   //!
        DES_CBC             = 18,
        DES_CBC_PKCS7       = 19,
        DES_CFB             = 20,
        DES_OFB             = 21,
        //CAST5_ECB           = 29,   //!
        CAST5_CBC           = 22,
        CAST5_CBC_PKCS7     = 23,
        CAST5_CFB           = 24,
        CAST5_OFB           = 25,
        TOTAL_NUMBER        = CAST5_OFB +1
    };
public:
    QStringList         getSuppotedFeatures () const;
    QStringList         getSuppotedTec      () const;
    QStringList         getProvidersList    () const;
    bool                isTecSupported      ( SymmetricMethods tec_code ) const;
    QString             getTecName          ( SymmetricMethods tec_code, bool b_short_name ) const;
    SymmetricMethods    getTectCode         ( const QString tec_name    ) const;
    //
    bool                encrypt_string      ( SymmetricMethods tec_code, const QString& str_password, const QString& str_input,     QString& str_output_64 );
    bool                decrypt_string      ( SymmetricMethods tec_code, const QString& str_password, const QString& str_input_64,  QString& str_output );
    //
    bool                encrypt_bytearray   ( SymmetricMethods tec_code, const QString& str_password, const QByteArray& array_input, QByteArray& array_output );
    bool                decrypt_bytearray   ( SymmetricMethods tec_code, const QString& str_password, const QByteArray& array_input, QByteArray& array_output );

private:
    QCA::Cipher::Mode   getMode             ( SymmetricMethods tec_code );
    QCA::Cipher::Padding getPadding         ( SymmetricMethods tec_code );

private:
    QString     m_strTecNames[TOTAL_NUMBER];
signals:
    
public slots:
    
};

#endif // CRYPTOSUPPORT_H
