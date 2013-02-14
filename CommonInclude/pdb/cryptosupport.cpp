#include "cryptosupport.h"
//
//
//
CryptoSupport::CryptoSupport(QObject *parent) :
    QObject(parent)
{
    m_strTecNames[NO_ENCRYPT] = "";
    //m_strTecNames[AES128_ECB]       = "aes128-ecb";
    m_strTecNames[AES128_CFB]       = "aes128-cfb";
    m_strTecNames[AES128_CBC]       = "aes128-cbc"; //
    m_strTecNames[AES128_CBC_PKCS7] = "aes128-cbc-pkcs7";
    m_strTecNames[AES128_OFB]       = "aes128-ofb";
    //m_strTecNames[AES192_ECB]       = "aes192-ecb";
    m_strTecNames[AES192_CFB]       = "aes192-cfb";
    m_strTecNames[AES192_CBC]       = "aes192-cbc";
    m_strTecNames[AES192_CBC_PKCS7] = "aes192-cbc-pkcs7";
    m_strTecNames[AES192_OFB]       = "aes192-ofb";
    //m_strTecNames[AES256_ECB]       = "aes256-ecb";
    m_strTecNames[AES256_CBC]       = "aes256-cbc";
    m_strTecNames[AES256_CBC_PKCS7] = "aes256-cbc-pkcs7";
    m_strTecNames[AES256_CFB]       = "aes256-cfb";
    m_strTecNames[AES256_OFB]       = "aes256-ofb";
    //m_strTecNames[BLOWFISH_ECB]     = "blowfish-ecb";
    m_strTecNames[BLOWFISH_CBC_PKCS7] = "blowfish-cbc-pkcs7";
    m_strTecNames[BLOWFISH_CBC]     = "blowfish-cbc";
    m_strTecNames[BLOWFISH_CFB]     = "blowfish-cfb";
    m_strTecNames[BLOWFISH_OFB]     = "blowfish-ofb";
    //m_strTecNames[TRIPLEDES_ECB]    = "tripledes-ecb";
    m_strTecNames[TRIPLEDES_CBC]    = "tripledes-cbc";
    //m_strTecNames[DES_ECB]          = "des-ecb";
    //m_strTecNames[DES_ECB_PKCS7]    = "des-ecb-pkcs7";
    m_strTecNames[DES_CBC]          = "des-cbc";
    m_strTecNames[DES_CBC_PKCS7]    = "des-cbc-pkcs7";
    m_strTecNames[DES_CFB]          = "des-cfb";
    m_strTecNames[DES_OFB]          = "des-ofb";
    //m_strTecNames[CAST5_ECB]        = "cast5-ecb";
    m_strTecNames[CAST5_CBC]        = "cast5-cbc";
    m_strTecNames[CAST5_CBC_PKCS7]  = "cast5-cbc-pkcs7";
    m_strTecNames[CAST5_CFB]        = "cast5-cfb";
    m_strTecNames[CAST5_OFB]        = "cast5-ofb";
}

QStringList CryptoSupport::getSuppotedFeatures () const
{
    QCA::Initializer init = QCA::Initializer();    //initialize QCA
    //
    QStringList features_list = QCA::supportedFeatures();
    //
    return features_list;
}

QStringList CryptoSupport::getSuppotedTec() const
{
    QCA::Initializer init = QCA::Initializer();     //initialize QCA
    QStringList tec_list;
    //
    QStringList features_list = QCA::supportedFeatures();
    //
    for (int i = 0; i < features_list.size(); ++i)
    {
        if (
                (features_list.at(i).contains("aes128")     == true) ||
                (features_list.at(i).contains("aes192")     == true) ||
                (features_list.at(i).contains("aes256")     == true) ||
                (features_list.at(i).contains("blowfish")   == true) ||
                (features_list.at(i).contains("des")        == true) || //tripledes and des
                (features_list.at(i).contains("cast5")      == true)
                //no rsa right now, only symmetric methods
           )
        {
            if (features_list.at(i).contains("ecb",Qt::CaseInsensitive) == false)
                tec_list << features_list.at(i); // it is not full list of tec, its is only some of it
        };
    };
    //
    return tec_list;
}

QStringList CryptoSupport::getProvidersList () const
{
    QCA::Initializer init = QCA::Initializer(); //initialize QCA
    //
    QStringList providers_list;
    //
    //QCA::scanForPlugins();
    QCA::ProviderList qcaProviders = QCA::providers();
    //
    QString info;
    //
    for (int i = 0; i < qcaProviders.size(); ++i)
    {
        providers_list<<qcaProviders[i]->name();
    };
    //
    return providers_list;
}

bool CryptoSupport::isTecSupported (SymmetricMethods tec_code) const
{
    if (tec_code >= TOTAL_NUMBER)
        return false;
    //
    QCA::Initializer init = QCA::Initializer(); //initialize QCA
    //
    if ( !QCA::isSupported( m_strTecNames[tec_code].toStdString().c_str() ) )
    {
        return false;
    };
    //
    return true;
}

QString   CryptoSupport::getTecName  ( SymmetricMethods tec_code, bool b_short_name ) const
{
    if (tec_code >= TOTAL_NUMBER)
        return m_strTecNames[NO_ENCRYPT];
    //
    if (b_short_name)
    {
        QString str_short_name = m_strTecNames[tec_code];
        str_short_name = str_short_name.section("-",0,0);
        return str_short_name;
    }
    //
    return m_strTecNames[tec_code];
}

CryptoSupport::SymmetricMethods CryptoSupport::getTectCode ( const QString tec_name    ) const
{
    for (unsigned int i = NO_ENCRYPT; i < TOTAL_NUMBER; i++ )
    {
        if (m_strTecNames[i].compare(tec_name) == 0)
            return (SymmetricMethods) i;
    };
    //
    return NO_ENCRYPT;
}

/*
        CBC, ///< operate in %Cipher Block Chaining mode
        CFB, ///< operate in %Cipher FeedBack mode
        ECB, ///< operate in Electronic Code Book mode
        OFB  ///< operate in Output FeedBack Mode
*/

QCA::Cipher::Mode CryptoSupport::getMode ( SymmetricMethods tec_code )
{
/*
    if (
        ( AES128_ECB        == tec_code) ||
        ( AES192_ECB        == tec_code) ||
        ( AES256_ECB        == tec_code) ||
        ( BLOWFISH_ECB      == tec_code) ||
        ( TRIPLEDES_ECB     == tec_code) ||
        ( DES_ECB           == tec_code) ||
        ( CAST5_ECB         == tec_code) ||
        ( DES_ECB_PKCS7     == tec_code)
       )
    {
        return QCA::Cipher::ECB;
    };
*/
    //
    if (
            ( AES128_CFB    == tec_code) ||
            ( AES192_CFB    == tec_code) ||
            ( AES256_CFB    == tec_code) ||
            ( BLOWFISH_CFB  == tec_code) ||
            ( DES_CFB       == tec_code) ||
            ( CAST5_CFB     == tec_code)
       )
    {
        return QCA::Cipher::CFB;
    };
    //
    if (
            ( AES128_CBC        == tec_code) ||
            ( AES128_CBC_PKCS7  == tec_code) ||
            ( AES192_CBC        == tec_code) ||
            ( AES192_CBC_PKCS7  == tec_code) ||
            ( AES256_CBC        == tec_code) ||
            ( AES256_CBC_PKCS7  == tec_code) ||
            ( BLOWFISH_CBC_PKCS7== tec_code) ||
            ( BLOWFISH_CBC      == tec_code) ||
            ( DES_CBC           == tec_code) ||
            ( DES_CBC_PKCS7     == tec_code) ||
            ( CAST5_CBC         == tec_code) ||
            ( CAST5_CBC_PKCS7   == tec_code) ||
            ( TRIPLEDES_CBC     == tec_code)
       )
    {
        return QCA::Cipher::CBC;
    };
    //
    /*
    AES128_OFB
    AES192_OFB
    AES256_OFB
    BLOWFISH_OFB
    DES_OFB
    CAST5_OFB
    */
    return QCA::Cipher::OFB;
}
/*
        DefaultPadding, ///< Default for cipher-mode
        NoPadding,      ///< Do not use padding
        PKCS7
*/
QCA::Cipher::Padding CryptoSupport::getPadding ( SymmetricMethods tec_code )
{
    if (
            ( AES128_CBC_PKCS7    == tec_code) ||
            ( AES192_CBC_PKCS7    == tec_code) ||
            ( AES256_CBC_PKCS7    == tec_code) ||
            ( BLOWFISH_CBC_PKCS7  == tec_code) ||
            //( DES_ECB_PKCS7       == tec_code) ||
            ( DES_CBC_PKCS7       == tec_code) ||
            ( CAST5_CBC_PKCS7     == tec_code)
       )
    {
        return QCA::Cipher::PKCS7;
    };
    //
    return QCA::Cipher::DefaultPadding;
}

bool CryptoSupport::encrypt_string ( SymmetricMethods tec_code, const QString& str_password, const QString& str_input, QString& str_output_64 )
{
    QCA::Initializer init = QCA::Initializer(); //initialize QCA
    //
    if (isTecSupported(tec_code) == false)
        return false;
    //
    if (str_password.length() == 0)
        return false;
    //
    if (tec_code == NO_ENCRYPT)
    {
        //TODO convert to 64
        str_output_64 = str_input;
        return true;
    };
    //
    QCA::SecureArray key(str_password.toStdString().c_str());
    QCA::SymmetricKey           s_key   = QCA::SymmetricKey(key);
    QCA::InitializationVector   iv      = QCA::InitializationVector(key);
    //
    const QString        str_tec_name = getTecName (tec_code, true);
    QCA::Cipher::Mode    mode         = getMode    (tec_code);
    QCA::Cipher::Padding pad          = getPadding (tec_code);
    //
    QCA::Cipher cipher = QCA::Cipher(str_tec_name,
                                       mode,
                                       pad,
                                       QCA::Encode,
                                       s_key,
                                       iv);

    //
    QCA::SecureArray secureData     = str_input.toAscii();
    //we encrypt the data
    QCA::SecureArray encryptedData  = cipher.process(secureData);
    //check if encryption succeded
    if ( !cipher.ok() )
    {
        return false;
    }
    //
    QString str_temp(encryptedData.toByteArray().toBase64());
    str_output_64 = str_temp;
    //display the result
    //
    return true;
}

bool CryptoSupport::decrypt_string ( SymmetricMethods tec_code, const QString& str_password, const QString& str_input_64, QString& str_output )
{
    QCA::Initializer init = QCA::Initializer(); //initialize QCA
    //
    if (isTecSupported(tec_code) == false)
        return false;
    //
    if (str_password.length() == 0)
        return false;
    //
    if (tec_code == NO_ENCRYPT)
    {
        //TODO convert from 64
        str_output = str_input_64;
        return true;
    };
    //
    QCA::SecureArray key(str_password.toStdString().c_str());
    QCA::SymmetricKey           s_key   = QCA::SymmetricKey(key);
    QCA::InitializationVector   iv      = QCA::InitializationVector(key);
    //
    QCA::Cipher cipher = QCA::Cipher(getTecName (tec_code,true),
                                     getMode    (tec_code),
                                     getPadding (tec_code),
                                     QCA::Decode,
                                     s_key,
                                     iv);
    //

    QByteArray array_temp(str_input_64.toAscii());

    QCA::SecureArray encryptedData = QByteArray::fromBase64( array_temp );
    //QCA::SecureArray encryptedData = str_input.toAscii();
    //
    //decrypt the encrypted data
    QCA::SecureArray decryptedData = cipher.process(encryptedData);
    //check if decryption succeded
    if (!cipher.ok())
    {
        return false;
    };
    //
    str_output = decryptedData.data();
    //
    return true;
}

bool CryptoSupport::encrypt_bytearray   ( SymmetricMethods tec_code, const QString& str_password, const QByteArray& array_input, QByteArray& array_output )
{
    QCA::Initializer init = QCA::Initializer(); //initialize QCA
    //
    if (isTecSupported(tec_code) == false)
        return false;
    //
    if (str_password.length() == 0)
        return false;
    //
    if (tec_code == NO_ENCRYPT)
    {
        array_output = array_input;
        return true;
    };
    //
    QCA::SecureArray key(str_password.toStdString().c_str());
    QCA::SymmetricKey           s_key   = QCA::SymmetricKey(key);
    QCA::InitializationVector   iv      = QCA::InitializationVector(key);
    //
    const QString        str_tec_name = getTecName (tec_code, true);
    QCA::Cipher::Mode    mode         = getMode    (tec_code);
    QCA::Cipher::Padding pad          = getPadding (tec_code);
    //
    QCA::Cipher cipher = QCA::Cipher(str_tec_name,
                                       mode,
                                       pad,
                                       QCA::Encode,
                                       s_key,
                                       iv);

    //
    QCA::SecureArray secureData     = array_input;
    //we encrypt the data
    QCA::SecureArray encryptedData  = cipher.process(secureData);
    //check if encryption succeded
    if ( !cipher.ok() )
    {
        return false;
    }
    //
    array_output = encryptedData.toByteArray();
    //
    return true;
}

bool CryptoSupport::decrypt_bytearray   ( SymmetricMethods tec_code, const QString& str_password, const QByteArray& array_input, QByteArray& array_output )
{
    QCA::Initializer init = QCA::Initializer(); //initialize QCA
    //
    if (isTecSupported(tec_code) == false)
        return false;
    //
    if (str_password.length() == 0)
        return false;
    //
    if (tec_code == NO_ENCRYPT)
    {
        array_output = array_input;
        return true;
    };
    //
    QCA::SecureArray key(str_password.toStdString().c_str());
    QCA::SymmetricKey           s_key   = QCA::SymmetricKey(key);
    QCA::InitializationVector   iv      = QCA::InitializationVector(key);
    //
    QCA::Cipher cipher = QCA::Cipher(getTecName (tec_code,true),
                                     getMode    (tec_code),
                                     getPadding (tec_code),
                                     QCA::Decode,
                                     s_key,
                                     iv);
    //
    QCA::SecureArray encryptedData = array_input;
    //
    //decrypt the encrypted data
    QCA::SecureArray decryptedData = cipher.process(encryptedData);
    //check if decryption succeded
    if (!cipher.ok())
    {
        return false;
    };
    //
    array_output = decryptedData.toByteArray();
    //
    return true;
}
