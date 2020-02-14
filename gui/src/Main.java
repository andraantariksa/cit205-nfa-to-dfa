import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.table.AbstractTableModel;
import java.util.*;
import java.util.stream.Collectors;

class NFATable extends AbstractTableModel {
    // TODO
    // Column names should be set
    private List<String> columnNames;
    private List<List<String>> data;

    NFATable() {
        this.data = new ArrayList<List<String>>();
        this.columnNames = new ArrayList<String>();

        this.columnNames.add("States");

        List<String> row = new ArrayList<String>();
        row.add("");
        this.data.add(row);
    }

    public void updateLanguage(String language) {
        this.columnNames.clear();
        this.columnNames.add("States");

        List<String> emptyStringRowData = new ArrayList<String>();
        emptyStringRowData.add("");

        for (char character: language.toCharArray()) {
            this.columnNames.add(Character.toString(character));

            emptyStringRowData.add("");
        }

        for (int i = 0; i < data.size(); ++i)
        {
            this.data.set(i, emptyStringRowData);
        }
        fireTableStructureChanged();
    }

    public String getColumnName(int i) {
        return columnNames.get(i);
    }

    @Override
    public boolean isCellEditable(int rowIndex, int columnIndex) {
        return true;
    }

    @Override
    public int getRowCount() {
        return this.data.size();
    }

    @Override
    public int getColumnCount() {
        try {
            return this.data.get(0).size();
        } catch (IndexOutOfBoundsException err) {
            return 0;
        }
    }

    @Override
    public Object getValueAt(int i, int i1) {
        return data.get(i).get(i1);
    }
}

public class Main {
    private JTextField textFieldLanguage;
    private JTable tableNFA;
    private JPanel jpanelMain;
    private NFATable tableNFAModel;

    public Main() {
        textFieldLanguage.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent documentEvent) {
                onLanguageChange();
            }

            @Override
            public void removeUpdate(DocumentEvent documentEvent) {
                onLanguageChange();
            }

            @Override
            public void changedUpdate(DocumentEvent documentEvent) {
                onLanguageChange();
            }
        });
    }

    private void onLanguageChange() {
        String language = textFieldLanguage.getText();
        String[] letters = language.split(",");
        Set<String> letterSet = Arrays.stream(letters).collect(Collectors.toSet());
        // TODO
        this.tableNFAModel.updateLanguage(language);
    }

    private void createUIComponents() {
        this.tableNFAModel = new NFATable();
        this.tableNFA = new JTable(this.tableNFAModel);
    }

    public static void main(String[] argsp) {
        JFrame jframe = new JFrame("NFA to DFA");
        jframe.setContentPane(new Main().jpanelMain);
        jframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        jframe.pack();
        jframe.setVisible(true);
    }
}
