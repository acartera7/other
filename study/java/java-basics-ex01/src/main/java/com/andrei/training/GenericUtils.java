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

}
