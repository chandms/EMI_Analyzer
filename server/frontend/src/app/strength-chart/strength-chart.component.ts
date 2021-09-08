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
  @Input() yLabel!: string;
  @Input() trendData!: Array<number>;

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
          unit: 'hour'
        }
      }],
      yAxes: [{
        scaleLabel: {
          display: true,
          labelString: ''
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
  lineChartType: ChartType  = 'line';

  constructor() { }

  ngOnInit(): void {
    this.lineChartData.push({
      data: this.strengthData,
      label: this.label,
      radius: 15,
      type: 'scatter'
    });
      this.lineChartData.push({
        data: this.trendData,
        borderColor: 'rgba(255,0,0,0.2)',
        type: 'line'
      });
    this.lineChartLabels = this.timeStamp;
    this.lineChartOptions.title.text = this.title;
    this.lineChartOptions.scales.yAxes[0].scaleLabel.labelString = this.yLabel;
  }

}
