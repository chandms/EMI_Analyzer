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
    this.service.getSweeps()
      .subscribe(Response => {
        console.log(Response);
        this.sweeps = Response;
      })
  }
}
