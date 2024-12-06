import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class day5 {

    private Map<Integer,List<Integer>> rules;
    private List<List<Integer>> updates;

    public day5() {

        this.rules = new java.util.HashMap<>();
        this.updates = new java.util.ArrayList<>();
        File file = new File("input.txt");
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            boolean rules = true;

            for (String line = br.readLine() ; line != null ; line = br.readLine()) {
                if (line.length() < 2) {
                    rules = false;
                    continue;
                }
                if (rules) {
                    String[] parts = line.split("[|]");
                    int key = Integer.parseInt(parts[0]);
                    int value = Integer.parseInt(parts[1]);
                    if (this.rules.containsKey(key)) {
                        this.rules.get(key).add(value);
                    }
                    else {
                        this.rules.put(key, new ArrayList<>(List.of(value)));
                    }
                }
                else {
                    String[] parts = line.split("[,]");
                    this.updates.add(Stream.of(parts).map(Integer::parseInt).collect(Collectors.toList()));
                }
            }

            br.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("File not found");
        } catch (IOException e) {
            System.out.println("IO Exception");
        }
    }

    public boolean check(List<Integer> update) {
        
        for (int i=1 ; i < update.size() ; i++) {
            if (this.rules.containsKey(update.get(i))) {
                for (Integer integer : this.rules.get(update.get(i))) {
                    int idx = update.indexOf(integer);
                    if (idx != -1 && idx < i) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    public void correct(List<Integer> update) {
        update.sort((a,b) -> {
            if (this.rules.containsKey(a) && this.rules.get(a).contains(b)) {
                return -1;
            }
            else if (this.rules.containsKey(b) && this.rules.get(b).contains(a)) {
                return 1;
            }
            return 0;
        });
    }

    public void Challenge1() {
        int sum = 0;
        for (List<Integer> update : this.updates) {
            if (this.check(update)) {
                sum += update.get(update.size() / 2);
            }
        }
        System.out.println("Challenge 1: " + sum);
    }

    public void Challenge2() {
        int sum = 0;
        for (List<Integer> update : this.updates) {
            if (!this.check(update)) {
                this.correct(update);
                sum += update.get(update.size() / 2);
            }
        }
        System.out.println("Challenge 2: " + sum);
    }
    

    public static void main(String[] args) {
        day5 app = new day5();
        app.Challenge1();
        app.Challenge2();
    }
}