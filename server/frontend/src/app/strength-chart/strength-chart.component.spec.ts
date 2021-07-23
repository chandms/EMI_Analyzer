import { ComponentFixture, TestBed } from '@angular/core/testing';

import { StrengthChartComponent } from './strength-chart.component';

describe('StrengthChartComponent', () => {
  let component: StrengthChartComponent;
  let fixture: ComponentFixture<StrengthChartComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ StrengthChartComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(StrengthChartComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
