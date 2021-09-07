import { Component, Input, OnInit } from '@angular/core';
import { ChartDataSets, ChartOptions, ChartType } from 'chart.js';
import { Color, Label } from 'ng2-charts';

@Component({
  selector: 'strength-chart',
  templateUrl: './strength-chart.component.html',
  styleUrls: ['./strength-chart.component.css']
})
export class StrengthChartComponent implements OnInit {

  @Input() deviceName!:string;
  @Input() strengthData!:Array<number>;
  @Input() timeStamp!: Array<Label>;
  @Input() title!: string;
  @Input() label!: string;

  lineChartData: ChartDataSets[] = [];
  lineChartLabels: Label[] = [];
  lineChartOptions: ChartOptions = {
    responsive: true,
    title: {
      display: true,
      text: this.title,
    },
    scales: {
      xAxes: [{
        type: 'time',
        time: {
          parser: 'MM/DD/YYYY, HH:mm:ss a',
          unit: 'day'
        }
      }]
    } 
  };
  lineChartColors: Color[] = [
    {
      borderColor: 'black',
      backgroundColor: 'rgba(255,255,0,0.2)',
    },
  ];
  lineChartLegend = false;
  lineChartPlugins = [];
  lineChartType: ChartType  = 'bubble';

  constructor() { }

  ngOnInit(): void {
    this.lineChartData.push({
      data: this.strengthData,
      label: this.label
    });
    this.lineChartLabels = this.timeStamp;
    this.lineChartOptions.title.text = this.title;
  }

}
