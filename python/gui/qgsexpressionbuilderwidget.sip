/** An expression item that can be used in the QgsExpressionBuilderWidget tree.
  */
class QgsExpressionItem : QStandardItem
{
%TypeHeaderCode
#include <qgsexpressionbuilderwidget.h>
%End

  public:
    enum ItemType
    {
      Header,
      Field,
      ExpressionNode
    };

    QgsExpressionItem( const QString& label,
                       const QString& expressionText,
                       const QString& helpText,
                       QgsExpressionItem::ItemType itemType = ExpressionNode );

    QgsExpressionItem( const QString& label,
                       const QString& expressionText,
                       QgsExpressionItem::ItemType itemType = ExpressionNode );

    QString getExpressionText() const;

    /** Get the help text that is associated with this expression item.
      *
      * @return The help text.
      */
    QString getHelpText() const;
    /** Set the help text for the current item
      *
      * @note The help text can be set as a html string.
      */
    void setHelpText( const QString& helpText );

    /** Get the type of expression item eg header, field, ExpressionNode.
      *
      * @return The QgsExpressionItem::ItemType
      */
    QgsExpressionItem::ItemType getItemType() const;

    //! Custom sort order role
    static const int CustomSortRole;
    //! Item type role
    static const int ItemTypeRole;
};

/** Search proxy used to filter the QgsExpressionBuilderWidget tree.
  * The default search for a tree model only searches top level this will handle one
  * level down
  */
class QgsExpressionItemSearchProxy : QSortFilterProxyModel
{
%TypeHeaderCode
#include <qgsexpressionbuilderwidget.h>
%End

  public:
    QgsExpressionItemSearchProxy();

    bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const;

  protected:

    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;
};


/** A reusable widget that can be used to build a expression string.
  * See QgsExpressionBuilderDialog for exmaple of usage.
  */
class QgsExpressionBuilderWidget : QWidget
{
%TypeHeaderCode
#include <qgsexpressionbuilderwidget.h>
%End

  public:
    QgsExpressionBuilderWidget( QWidget *parent /TransferThis/ = nullptr );
    ~QgsExpressionBuilderWidget();

    /** Sets layer in order to get the fields and values
      * @note this needs to be called before calling loadFieldNames().
      */
    void setLayer( QgsVectorLayer* layer );

    /** Loads all the field names from the layer.
      * @remarks Should this really be public couldn't we just do this for the user?
      */
    void loadFieldNames();

    void loadFieldNames( const QgsFields& fields );

    /** Loads field names and values from the specified map.
     *  @note The field values must be quoted appropriately if they are strings.
     *  @note added in QGIS 2.12
     */
    void loadFieldsAndValues( const QMap<QString, QStringList>& fieldValues );

    /** Sets geometry calculator used in distance/area calculations. */
    void setGeomCalculator( const QgsDistanceArea & da );

    /** Gets the expression string that has been set in the expression area.
      * @returns The expression as a string. */
    QString expressionText();

    /** Sets the expression string for the widget */
    void setExpressionText( const QString& expression );

    /** Returns the expression context for the widget. The context is used for the expression
     * preview result and for populating the list of available functions and variables.
     * @see setExpressionContext
     * @note added in QGIS 2.12
     */
    QgsExpressionContext expressionContext() const;

    /** Sets the expression context for the widget. The context is used for the expression
     * preview result and for populating the list of available functions and variables.
     * @param context expression context
     * @see expressionContext
     * @note added in QGIS 2.12
     */
    void setExpressionContext( const QgsExpressionContext& context );

    /** Registers a node item for the expression builder.
      * @param group The group the item will be show in the tree view.  If the group doesn't exsit it will be created.
      * @param label The label that is show to the user for the item in the tree.
      * @param expressionText The text that is inserted into the expression area when the user double clicks on the item.
      * @param helpText The help text that the user will see when item is selected.
      * @param type The type of the expression item.
      * @param highlightedItem set to true to make the item highlighted, which inserts a bold copy of the item at the top level
      * @param sortOrder sort ranking for item
      */
    void registerItem( const QString& group, const QString& label, const QString& expressionText,
                       const QString& helpText = "",
                       QgsExpressionItem::ItemType type = QgsExpressionItem::ExpressionNode,
                       bool highlightedItem = false, int sortOrder = 1 );

    bool isExpressionValid();

    void saveToRecent( const QString& key );

    void loadRecent( const QString& key );

    /** Create a new file in the function editor
     */
    void newFunctionFile( const QString& fileName = "scratch" );

    /** Save the current function editor text to the given file.
     */
    void saveFunctionFile( QString fileName );

    /** Load code from the given file into the function editor
     */
    void loadCodeFromFile( QString path );

    /** Load code into the function editor
     */
    void loadFunctionCode( const QString& code );

    /** Update the list of function files found at the given path
     */
    void updateFunctionFileList( const QString& path );

  public slots:

    /**
     * Load sample values into the sample value area
     */
    void loadSampleValues();

    /**
     * Load all unique values from the set layer into the sample area
     */
    void loadAllValues();

    /**
     * Auto save the current Python function code.
     */
    void autosave();

    /**
     * Enabled or disable auto saving. When enabled Python scripts will be auto saved
     * when text changes.
     * @param enabled True to enable auto saving.
     */
    void setAutoSave( bool enabled );

  signals:
    /** Emitted when the user changes the expression in the widget.
      * Users of this widget should connect to this signal to decide if to let the user
      * continue.
      * @param isValid Is true if the expression the user has typed is valid.
      */
    void expressionParsed( bool isValid );
};
