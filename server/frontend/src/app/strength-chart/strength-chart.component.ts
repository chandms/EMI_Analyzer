import { Component, OnInit } from '@angular/core';
import { ChartDataSets, ChartType } from 'chart.js';
import { Color, Label } from 'ng2-charts';

@Component({
  selector: 'strength-chart',
  templateUrl: './strength-chart.component.html',
  styleUrls: ['./strength-chart.component.css']
})
export class StrengthChartComponent implements OnInit {

  lineChartData: ChartDataSets[] = [
    { data: [1, 2, 4, 5, 8, 10], label: 'Strenght' },
  ];
  lineChartLabels: Label[] = ['Day 0', 'Day 1', 'Day 2', 'Day 3', 'Day 4', 'Day 5'];
  lineChartOptions = {
    responsive: true,
  };
  lineChartColors: Color[] = [
    {
      borderColor: 'black',
      backgroundColor: 'rgba(255,255,0,0.2)',
    },
  ];
  lineChartLegend = false;
  lineChartPlugins = [];
  lineChartType: ChartType  = 'line';

  constructor() { }

  ngOnInit(): void {
  }

}
