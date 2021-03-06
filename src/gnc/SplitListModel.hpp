/*
 * SplitListModel.hpp
 * Copyright (C) 2010 Christian Stimming
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, contact:
 *
 * Free Software Foundation           Voice:  +1-617-542-5942
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652
 * Boston, MA  02110-1301,  USA       gnu@gnu.org
 */

#ifndef GNC_SPLITLISTMODEL_HPP
#define GNC_SPLITLISTMODEL_HPP

#include "config.h"
#include "gncmm/Account.hpp"
#include "gncmm/Split.hpp"
#include "gnc/QofEventWrapper.hpp"
#include "gncmm/Transaction.hpp"
#include "gnc/conv.hpp"

extern "C"
{
#include "engine/Transaction.h"
}

#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QHash>
class QUndoStack;

namespace gnc
{

/** This is the data model for a list of splits.
 */
class SplitListModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    enum ColumnNumbers
    {
        COLUMN_DATE = 0
        , COLUMN_NUM
        , COLUMN_DESC
        , COLUMN_ACCOUNT
        , COLUMN_RECONCILE
        , COLUMN_INCREASE
        , COLUMN_DECREASE
        , COLUMN_BALANCE
        , COLUMN_LAST
    };

    SplitListModel(const Glib::RefPtr<Account> acc, QUndoStack* undoStack, QObject *parent = 0);
    ~SplitListModel();

    Glib::RefPtr<Account> getAccount() const { return m_account; }

    QModelIndex parent(const QModelIndex &index) const { return QModelIndex(); }
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

public Q_SLOTS:
    void transactionEvent( ::Transaction* trans, QofEventId event_type);
    void accountEvent( ::Account* trans, QofEventId event_type);
    void editorClosed(const QModelIndex& index, QAbstractItemDelegate::EndEditHint hint);

private:
    void recreateCache();
    void recreateTmpTrans();

protected:
    Glib::RefPtr<Account> m_account;
    SplitQList m_list;
    QUndoStack* m_undoStack;
    typedef QHash< ::Transaction*, int> TransactionRowHash;
    TransactionRowHash m_hash;

    /** The wrapper for receiving events from gnc. */
    QofEventWrapper<SplitListModel, ::Transaction*> m_eventWrapper;
    QofEventWrapper<SplitListModel, ::Account*> m_eventWrapperAccount;

    bool m_enableNewTransaction;
    TmpTransaction m_tmpTransaction;
};

} // END namespace gnc

#endif
