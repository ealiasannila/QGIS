class QgsVectorRandomColorRampDialog : QDialog
{
%TypeHeaderCode
#include <qgsvectorrandomcolorrampdialog.h>
%End

  public:
    QgsVectorRandomColorRampDialog( QgsVectorRandomColorRamp* ramp, QWidget* parent /TransferThis/ = NULL );

  public slots:
    void setCount( int val );
    void setHue1( int val );
    void setHue2( int val );
    void setSat1( int val );
    void setSat2( int val );
    void setVal1( int val );
    void setVal2( int val );

  protected:

    void updatePreview();
};
