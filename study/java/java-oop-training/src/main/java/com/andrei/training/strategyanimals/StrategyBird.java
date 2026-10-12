package com.andrei.training.strategyanimals;

import com.andrei.training.behaviors.call.CallBehavior;
import com.andrei.training.behaviors.movement.MovementBehavior;

public class StrategyBird extends StrategyAnimal {

    public StrategyBird(String name,
                        int age,
                        float weight,
                        String variation,
                        CallBehavior callBehavior,
                        MovementBehavior movementBehavior) {

        super(name, age, weight, variation);
    }


}
