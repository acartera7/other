package com.andrei.training;

import java.util.*;

public class GenericUtils {

    public static <T> void printAll(Collection<? extends Box<T>> items ) {
        for (Box<T> item : items) {
            System.out.println(item.toString());
        }
    }

    public static double sumNumbers (List<? extends Number> items) {
        return items.stream().mapToDouble(Number::doubleValue).sum();
    }

    public static void addIntegers (List<? super Integer> items, List<Integer> integersToAdd) {
        items.addAll(integersToAdd);
    }

    public static void run() {
        Box<String> s_box1 = new Box<>("Test1");
        Box<Integer> i_box1 = new Box<>(4);

        System.out.println(s_box1);
        System.out.println(i_box1);

        Pair<Integer,String> pair1 = new Pair<>(1,"one");
        Pair<Integer,String> pair2 = new Pair<>(2,"two");

        System.out.println(pair1);
        System.out.println(pair2);

        List<Box<Integer>> ibox_list = new ArrayList<>(List.of(
                new Box<>(2),
                new Box<>(5),
                new Box<>(7)
        ));

        printAll(ibox_list);

        List<Float> floatList = new ArrayList<>(List.of(
                4.0f,
                5.2f,
                1.1f,
                12.2f
        ));

        var result = sumNumbers(floatList);

        System.out.println("\nList of floats: ");
        System.out.println(floatList);

        System.out.println("\nSum of floats: " + sumNumbers(floatList));

        List<Integer> integerList = new ArrayList<>(List.of(
                4,
                5,
                1,
                7
        ));

        System.out.println("List of integers: ");
        System.out.println(integerList);

        System.out.printf("\nAdding the following integers to the list: %d, %d, %d\n", 3,5,7);
        addIntegers(integerList, List.of(3,5,7));
        System.out.println(integerList);

    }

}
