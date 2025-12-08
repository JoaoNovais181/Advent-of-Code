
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

class Problem {
    private List<List<Character>> map;
    private int robotX, robotY;
    private char[] moves;
    private Map<Character, int[]> directions = new HashMap<>(); 

    public Problem() {
        this.map = new ArrayList<>();
        
        File file = new File("input.txt");
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {

            boolean is_map = true;
            String moves_string = "";
            directions.put('<', new int[]{0,-1});
            directions.put('>', new int[]{0,1});
            directions.put('^', new int[]{-1,0});
            directions.put('v', new int[]{1,0});
            
            for (String line = br.readLine() ; line != null ; line = br.readLine()) {
                
                if (line.length() <= 1) {
                    is_map = false;
                    continue;
                }

                if (is_map) {
                    if (line.contains("@")) {
                        robotX = line.indexOf("@");
                        robotY = map.size();
                    }

                    List<Character> l = new ArrayList<>();
                    for (char c : line.toCharArray())
                        l.add(c);
                    map.add(l);
                }
                else {
                    moves_string += line.trim();
                }
            }

            moves = moves_string.toCharArray();

            br.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("File not found");
        } catch (IOException e) {
            System.out.println("IO Exception");
        }
    }

    private long gps() {
        long r = 0;

        for (int i=0 ; i < map.size() ; i++)
            for (int j=0 ; j < map.get(i).size() ; j++)
                if (map.get(i).get(j) == 'O')
                    r += i*100 + j;

        return r;
    }

    public void swap(int fstI, int fstJ, int sndI, int sndJ) {
        var fstLine = map.get(fstI);
        var sndLine = map.get(sndI);

        char tmp = fstLine.get(fstJ);
        fstLine.set(fstJ, sndLine.get(sndJ));
        sndLine.set(sndJ, tmp);
    }

    private void simulate_move(int posI, int posJ, char move) {
        char curr = map.get(posI).get(posJ);
        if (curr == '#')
            return;
        
        int[] dir = directions.get(move);
        int[] new_pos = new int[]{posI + dir[0], posJ + dir[1]};

        if (new_pos[0] >= 0 && new_pos[0] < map.size() && new_pos[1] >= 0 && new_pos[1] < map.get(0).size())
            simulate_move(posI + dir[0], posJ + dir[1], move);

        if (map.get(posI-dir[0]).get(posJ-dir[1]) == '.')
            return;

        if (curr == '.') {

            System.out.println("OI");
            swap(posI, posJ, posI - dir[0], posJ - dir[1]);

            if (posI - dir[0] == robotY && posJ - dir[1] == robotX) {
                robotY = posI;
                robotX = posJ;
            }
        }

        // if (curr == '.') {
        //     System.out.println("OI");
        //     swap(posI, posJ, posI - dir[0], posJ - dir[1]);

        //     if (posI - dir[0] == robotY && posJ - dir[1] == robotX) {
        //         robotY = posI;
        //         robotX = posJ;
        //     }
        // }
    }

    private void print_map() {
        for (var l : map)
            System.out.println(l);
        System.out.println();
    }

    private void challenge1() {
        print_map();
        for (char move : moves) {
            System.out.println("Move " + move + ":");
            simulate_move(robotY, robotX, move);
            print_map();
        }

        System.out.println("Challenge1: " + gps());
    }

    public void solve() {
        var copy = new ArrayList<>();
        for (var l : map)
            copy.add(new ArrayList<>(l));

        challenge1();
    }
}

public class Day15 {

    

    public static void main(String[] args) {
        Problem p = new Problem();

        p.solve();
    }
}