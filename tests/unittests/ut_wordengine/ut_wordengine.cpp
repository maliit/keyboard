

#include "models/wordribbon.h"
#include "plugin/inputmethod.h"
#include "common/inputmethodhostprobe.h"

#include <maliit/plugins/abstractinputmethodhost.h>

#include <QtTest/QtTest>

using namespace MaliitKeyboard;


class TestWordEngine: public QObject
{
  Q_OBJECT
private:

  Q_SLOT void wordRibbon() {

    // WordRibbon is a QAbstractListModel, exposed to QML

    WordRibbon wr;

    QCOMPARE(wr.candidates().size(), 0);

    WordCandidate wc1(WordCandidate::SourceUnknown, "word_1");
    WordCandidate wc2(WordCandidate::SourceUnknown, "word_2");
    WordCandidate wc3(WordCandidate::SourceUnknown, "word_3");

    wr.appendCandidate(wc1);
    wr.appendCandidate(wc2);
    wr.appendCandidate(wc3);

    QCOMPARE( wr.data( wr.index(0,0), WordRibbon::WordRole ), QVariant("word_1"));
    QCOMPARE( wr.data( wr.index(1,0), WordRibbon::WordRole ), QVariant("word_2"));
    QCOMPARE( wr.data( wr.index(2,0), WordRibbon::WordRole ), QVariant("word_3"));

    QCOMPARE( wr.rowCount(), 3 );


    QVector<WordCandidate> candidates = wr.candidates();
    QCOMPARE( candidates.size(), 3 );
    QCOMPARE( candidates[0], wc1 );
    QCOMPARE( candidates[1], wc2 );
    QCOMPARE( candidates[2], wc3 );

    wr.clearCandidates();
    QCOMPARE( wr.rowCount(), 0 );

    wr.appendCandidate(wc1);
    wr.appendCandidate(wc2);
    wr.appendCandidate(wc3);

    QCOMPARE( wr.rowCount(), 3 );

    // initially set to false
    QCOMPARE( wr.enabled(), false );

    wr.setEnabled(true);
    QCOMPARE( wr.enabled(), true );

    wr.setEnabled(false);
    QCOMPARE( wr.enabled(), false );

    // later
    QCOMPARE( wr.valid(), false );

    WordCandidate w4(WordCandidate::SourceUnknown, "another_word");

    // make sure model-related signals work
    QSignalSpy modelAboutToBeResetSpy(&wr, SIGNAL( modelAboutToBeReset() ));
    QSignalSpy rowsInsertedSpy(&wr, SIGNAL( rowsInserted(QModelIndex, int, int) ) );

    QVERIFY(modelAboutToBeResetSpy.isValid());
    QVERIFY(rowsInsertedSpy.isValid());

    wr.appendCandidate(w4);

    QCOMPARE(rowsInsertedSpy.count(), 1);

    wr.clearCandidates();

    QCOMPARE( modelAboutToBeResetSpy.count(), 1 );

    QList<QVariant> arguments = rowsInsertedSpy.takeFirst();
    QVERIFY(arguments[1].toInt() == 3);
    QVERIFY(arguments[2].toInt() == 3);

    /*
      this API should be reviewed and refactored where appropriate:

        valid
        rect
        origin/setOrigin   - currently not used
        rCandidates        - returns reference, not copy. no const. not documented
        area/rArea/setArea - currently not used
        Q_PROPERTY enabled - currently not used
        operator==
        operator!=
    */
  }
  
};


QTEST_MAIN(TestWordEngine)
#include "ut_wordengine.moc"
