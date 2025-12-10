
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Day6 {

    public class Operation {

        private final char op;

        public Operation(char op) {
            this.op = op;
        }

        Long performOperation(Long num1, Long num2) {
            switch (op) {
                case '+' -> {
                    return num1 + num2;
                }
                case '-' -> {
                    return num1 - num2;
                }
                case '*' -> {
                    return num1 * num2;
                }
                case '/' -> {
                    return num1 / num2;
                }
            }
            return (long)0;
        }

        public long getStart() {
            if (op == '+' || op == '-')
                return 0;
            return 1;
        }

        public char getChar() {
            return op;
        }
    }

    private List<List<Long>> problems1, problems2;
    private List<Operation> operations;

    private String addPadding(String string, int padding, boolean right) {
        if (right) {
            StringBuilder sb = new StringBuilder(string);
            for (int i=0 ; i<padding ; i++) {
                sb.append('0');
            }
            return sb.toString();
        }
        StringBuilder sb = new StringBuilder();
        for (int i=0 ; i<padding ; i++) {
            sb.append('0');
        }
        sb.append(string);
        return sb.toString();
    }

    public Day6(String inputFile) throws FileNotFoundException, IOException {

        this.problems1 = new ArrayList<>();
        this.problems2 = new ArrayList<>();
        List<List<Long>> auxProblems1 = new ArrayList<>();
        List<String> auxProblems2 = new ArrayList<>();
        this.operations = new ArrayList<>();

        try (BufferedReader fileReader = new BufferedReader(new FileReader(inputFile))) {
            String line = fileReader.readLine();
            
            while (line != null) {
                if (line.length() == 0)
                    break;
                auxProblems2.add(line);
                line = line.trim();
                String[] parts = line.split("\s+");
                

                // if it is operations line
                if (parts[0].length() == 1 && "+-*/".contains(parts[0])) {
                    for (String op : parts) {
                        operations.add(new Operation(op.charAt(0)));
                    }
                }
                else {
                    List<Long> problem = new ArrayList<>();
                    for (String num : parts) {
                        problem.add(Long.valueOf(num));
                    }
                    auxProblems1.add(problem);
                }
                line = fileReader.readLine();
            }
        }

        // transpose number matrix (for challenge 1)
        for (int i=0 ; i<auxProblems1.get(0).size() ; i++) {
            List<Long> problem = new ArrayList<>();
            // parse input for challenge 1
            for (List<Long> pr : auxProblems1)
                problem.add(pr.get(i));
            problems1.add(problem);
        }

        // parse input for challenge 2
        List<Integer> begginings = new ArrayList<>();
        
        String operationsString = auxProblems2.remove(auxProblems2.size()-1);

        for (int i=0 ; i<operationsString.length() ; i++)
            if ("+-*/".indexOf(operationsString.charAt(i)) != -1)
                begginings.add(i);

        for (int i=0 ; i<begginings.size() ; i++) {
            int current = begginings.get(i);
            int next = operationsString.length() - 1;
            if (i != begginings.size() - 1)
                next = begginings.get(i+1) - 2;

            List<Long> problem = new ArrayList<>();
            for (int j=next ; j>=current ; j--) {
                long num = 0;
                for (String str : auxProblems2) {
                    char ch = str.charAt(j);
                    if (ch == ' ')
                        continue;
                    num = num * 10 + (ch - '0');
                }
                problem.add(num);
            }
            problems2.add(problem);
        }
    }

    public void challenge1() {
        long grandTotal = 0;

        for (int i=0 ; i<operations.size() ; i++) {
            List<Long> problem = problems1.get(i);
            Operation op = operations.get(i);
            long total = problem.stream().reduce(op.getStart(), op::performOperation);
            grandTotal += total;
        }

        System.out.println("[Challenge1] The grand total is " + grandTotal);
    }

    public void challenge2() {
        long grandTotal = 0;

        for (int i=0 ; i<operations.size() ; i++) {
            List<Long> problem = problems2.get(i);
            Operation op = operations.get(i);
            long total = problem.stream().reduce(op.getStart(), op::performOperation);
            grandTotal += total;
        }

        System.out.println("[Challenge1] The grand total is " + grandTotal);
    }

    public static void main(String[] args) throws FileNotFoundException, IOException {
        Day6 solution = new Day6("input.txt");
        solution.challenge1();
        solution.challenge2();
    }
}
