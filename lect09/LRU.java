import java.util.ArrayList;
import java.util.List;

public class LRU {
    public static void main(String[] args) {
        List<Integer> pages = new ArrayList<>();

        int size = 3;
        int accesses[] = new int[]{1, 2, 3, 4};

        for (int access : accesses) {
            if (pages.contains(access)) {
                pages.remove(access);
                pages.add(access);
            } else {
                if (pages.size() == size) {
                    pages.remove(0);
                }
                pages.add(access);
            }
            System.out.println(pages);
        }
    }
}
