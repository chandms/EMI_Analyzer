import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceTempComponent } from './device-temp.component';

describe('DeviceTempComponent', () => {
  let component: DeviceTempComponent;
  let fixture: ComponentFixture<DeviceTempComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DeviceTempComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceTempComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
