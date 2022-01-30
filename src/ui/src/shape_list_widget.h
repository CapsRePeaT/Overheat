#pragma once

#include <QDockWidget>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>
#include <QTreeView>


class TreeModel;

// top level item should be a design
class ShapeListWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit ShapeListWidget(QWidget* parent = nullptr);
	~ShapeListWidget() = default;

 private:
	void Test();
	TreeModel* model_ = nullptr;
	QTreeView* view_ = nullptr;
};

class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr);
		~TreeItem() { qDeleteAll(m_childItems); }
    void append_child(TreeItem* item) { m_childItems.append(item); }
    TreeItem *child(int row);
		int child_count() const { return m_childItems.count(); }
		int column_count() const { return m_itemData.count(); }
    QVariant data(int column) const;
    int row() const;
		TreeItem* parentItem() { return m_parentItem; }
	 private:
    QVector<TreeItem*> m_childItems;
    QVector<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

class TreeModel : public QAbstractItemModel {
	Q_OBJECT

 public:
	explicit TreeModel(QObject* parent = nullptr);
	~TreeModel() { delete rootItem; }

	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
	                  const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	void TestFillWithTxtFile(const QString& file_path);
 private:

	TreeItem* rootItem;
};

