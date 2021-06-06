import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { SweepService } from './sweep.service';

@Component({
  selector: 'sweep',
  templateUrl: './sweep.component.html',
  styleUrls: ['./sweep.component.css']
})
export class SweepComponent implements OnInit {
  sweeps :any;

  constructor(private service: SweepService) {}

  ngOnInit() {
    this.service.getAllSweeps()
      .subscribe(Response => {
        this.sweeps = Response;
      })
  }
}
