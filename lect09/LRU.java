import java.util.ArrayList;
import java.util.List;

public class LRU {
    public static void main(String[] args) {
        List<Integer> pages = new ArrayList<>();

        int size = 4;
        int accesses[] = new int[]{0, 3, 2, 0, 1, 3, 4, 3, 1, 0, 3, 2, 1, 3, 4};

        for (int access : accesses) {
            if (pages.contains(new Integer(access))) {
                pages.remove(new Integer(access));
            } else if (pages.size() == size) {
                pages.remove(0);
            }
            pages.add(new Integer(access));
            System.out.println(pages);
        }
    }
}
