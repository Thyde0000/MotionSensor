package com.thydepods.motionsensorapi.controller;

import com.thydepods.motionsensorapi.model.MotionEvent;
import com.thydepods.motionsensorapi.repository.MotionRepository;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@CrossOrigin(origins = "http://localhost:5173")
@RestController
@RequestMapping("/api/motion")
public class MotionController {

    private final MotionRepository motionRepository;

    public MotionController(MotionRepository motionRepository) {
        this.motionRepository = motionRepository;
    }

    @GetMapping()
    public List<MotionEvent> getAllMotionEvents() {
        List<MotionEvent> events = motionRepository.findAll();
        for (MotionEvent e : events) {
            System.out.println(e.getId() + " " + e.getEventTime() + " " + e.getSensorStatus());
        }
        return motionRepository.findAll();
    }

    @DeleteMapping("/{id}")
    public void deleteMotionEvent(@PathVariable Long id) {
        motionRepository.deleteById(id);
    }
}
