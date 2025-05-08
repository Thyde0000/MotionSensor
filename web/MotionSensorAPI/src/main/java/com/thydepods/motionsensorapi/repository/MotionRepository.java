package com.thydepods.motionsensorapi.repository;

import com.thydepods.motionsensorapi.model.MotionEvent;
import org.springframework.data.jpa.repository.JpaRepository;

public interface MotionRepository extends JpaRepository<MotionEvent, Long> {
}
